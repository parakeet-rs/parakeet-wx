#include "um-crypto/decryption/kugou/KugouFileLoader.h"

#include "internal/XorHelper.h"
#include "um-crypto/endian.h"
#include "um-crypto/utils/StringHelper.h"

#include <cassert>

namespace umc::decryption::kugou {
// Private implementation

/**
 * @brief Kugou encryption type.
 */
enum class KugouCryptoType {
  /**
   * @brief Standard version of its crypto.
   */
  kKGM = 0,

  /**
   * @brief Hi-Res version of the same crypto,
   *        with an extra layer of post-processing.
   */
  kVPR,
};

namespace detail {

constexpr usize kFileMagicSize = 0x10;
constexpr usize kMinimalHeaderSize = 0x2c;
typedef Arr<u8, 17> KugouFileKey;

const Arr<u8, kFileMagicSize> kKGMFileMagic = {
    0x7c, 0xd5, 0x32, 0xeb, 0x86, 0x02, 0x7f, 0x4b,
    0xa8, 0xaf, 0xa6, 0x8e, 0x0f, 0xff, 0x99, 0x14,
};

const Arr<u8, kFileMagicSize> kVPRFileMagic = {
    0x05, 0x28, 0xbc, 0x96, 0xe9, 0xe4, 0x5a, 0x43,
    0x91, 0xaa, 0xbd, 0xd0, 0x7a, 0xf5, 0x36, 0x31,
};

enum class State {
  kReadFileMagic = 0,
  kWaitForHeader,
  kSeekToBody,
  kDecrypt,
};

template <KugouCryptoType Type, bool UseCache>
class KugouFileLoaderImpl : public KugouFileLoader {
 public:
  KugouFileLoaderImpl(const KugouInternalTable& t1,
                      const KugouInternalTable& t2,
                      const KugouInternalTable& v2,
                      const KugouVPRKey& vpr_key,
                      const char* subtype)
      : t1_(t1), t2_(t2), v2_(v2), vpr_key_(vpr_key) {
    const char* kCacheType = UseCache ? "cache" : "no-cache";

    name_ = utils::Format("Kugou(%s,%s)", subtype, kCacheType);
  }
  virtual const Str GetName() const override { return name_; };

 private:
  Str name_;
  State state_ = State::kReadFileMagic;

  KugouInternalTable t1_;
  KugouInternalTable t2_;
  KugouInternalTable v2_;
  KugouVPRKey vpr_key_;

#define T1(X) (t1_[(X) % kKugouInternalTableSize])
#define T2(X) (t2_[(X) % kKugouInternalTableSize])
#define V2(X) (v2_[(X) % kKugouInternalTableSize])
#define VPRKEY(X) (vpr_key_[(X) % kKugouVprKeySize])

  usize header_size_ = 0x400;
  KugouFileKey file_key_;

  Vec<u8> cache_;
  inline void PopulateCacheUntil(usize required_offset) {
    if constexpr (!UseCache) return;

    // 4M
    constexpr usize kIncrease = 4 * 1024 * 1024;

    // We either double existing buffer, or get it until the offset.
    usize current_size = cache_.size();
    if (current_size > required_offset) return;

    if (required_offset % kIncrease > 0) {
      required_offset += kIncrease - (required_offset % kIncrease);
    }

    const usize new_size = std::max(current_size + kIncrease, required_offset);
    cache_.resize(new_size);

    u8* p_cache = cache_.data();
    for (usize i = current_size; i < new_size; i++) {
      p_cache[i] = T1(i) ^ T2(i >> 4) ^ p_cache[i >> 8];
    }
  }

  inline u8 DecryptByte(u8 value, usize offset) {
    value ^= V2(offset);
    value ^= file_key_[offset % 17];

    if constexpr (UseCache) {
      // Optimisation using large cache
      value ^= cache_[offset >> 4];
    } else {
      usize iter_offset = offset;
      while (iter_offset) {
        iter_offset >>= 4;
        value ^= T1(iter_offset);
        iter_offset >>= 4;
        value ^= T2(iter_offset);
      }
    }

    value = value ^ (value << 4);

    if constexpr (Type == KugouCryptoType::kVPR) {
      value ^= VPRKEY(offset);
    }

    return value;
  }

  inline void DecryptInput(const u8*& in, usize& len) {
    auto p_out = ExpandOutputBuffer(len);

    usize offset = offset_ - header_size_;
    PopulateCacheUntil(offset + len);
    for (usize i = 0; i < len; i++) {
      p_out[i] = DecryptByte(in[i], offset + i);
    }

    offset_ += len;
    len = 0;
  }

  bool Write(const u8* in, usize len) override {
    while (len) {
      switch (state_) {
        case State::kReadFileMagic:
          if (ReadUntilOffset(in, len, kFileMagicSize)) {
            bool header_magic_match = false;
            if constexpr (Type == KugouCryptoType::kKGM) {
              header_magic_match = std::equal(
                  kKGMFileMagic.begin(), kKGMFileMagic.end(), buf_in_.begin());
            } else if constexpr (Type == KugouCryptoType::kVPR) {
              header_magic_match = std::equal(
                  kVPRFileMagic.begin(), kVPRFileMagic.end(), buf_in_.begin());
            } else {
              error_ = "invalid file type enum";
              return false;
            }

            if (!header_magic_match) {
              error_ = "file header magic not found";
              return false;
            }

            state_ = State::kWaitForHeader;
          }
          break;

        case State::kWaitForHeader:
          if (ReadUntilOffset(in, len, kMinimalHeaderSize)) {
            header_size_ = ReadLittleEndian<u32>(&buf_in_[0x10]);

            std::copy_n(&buf_in_[0x1c], 0x10, file_key_.begin());
            file_key_[0x10] = 0;

            state_ = State::kSeekToBody;
          }
          break;

        case State::kSeekToBody:
          if (ReadUntilOffset(in, len, header_size_)) {
            state_ = State::kDecrypt;
          }
          break;

        case State::kDecrypt:
          DecryptInput(in, len);
          break;
      }
    }

    assert(len == 0);

    return true;
  }

  bool End() {
    if constexpr (UseCache) {
      cache_.resize(0);
      cache_.shrink_to_fit();
    }
    return true;
  }
};

}  // namespace detail

// Public interface

#define MAKE_KUGOU_IMPL(NAME)                                                  \
  (use_cache                                                                   \
       ? static_cast<std::unique_ptr<KugouFileLoader>>(                        \
             std::make_unique<                                                 \
                 detail::KugouFileLoaderImpl<KugouCryptoType::k##NAME, true>>( \
                 t1, t2, v2, vpr_key, #NAME))                                  \
       : static_cast<std::unique_ptr<KugouFileLoader>>(                        \
             std::make_unique<detail::KugouFileLoaderImpl<                     \
                 KugouCryptoType::k##NAME, false>>(t1, t2, v2, vpr_key,        \
                                                   #NAME)))

std::unique_ptr<KugouFileLoader> KugouFileLoader::Create(
    const KugouInternalTable& t1,
    const KugouInternalTable& t2,
    const KugouInternalTable& v2,
    bool use_cache) {
  KugouVPRKey vpr_key{};
  return MAKE_KUGOU_IMPL(KGM);
}

std::unique_ptr<KugouFileLoader> KugouFileLoader::Create(
    const KugouInternalTable& t1,
    const KugouInternalTable& t2,
    const KugouInternalTable& v2,
    const KugouVPRKey& vpr_key,
    bool use_cache) {
  return MAKE_KUGOU_IMPL(VPR);
}

}  // namespace umc::decryption::kugou
