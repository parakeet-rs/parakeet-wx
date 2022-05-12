#include "um-crypto/decryption/kuwo/KuwoFileLoader.h"
#include "um-crypto/endian.h"
#include "um-crypto/utils/StringHelper.h"

#include "../../internal/XorHelper.h"

#include <cinttypes>

namespace umc::decryption::kuwo {

namespace detail {

constexpr usize kFileHeaderSize = 0x20;
constexpr usize kFileKeyOffset = 0x18;
constexpr usize kFullHeaderSize = 0x400;

const Arr<u8, 0x10> kKuwoMagicHeader = {
    0x79, 0x65, 0x65, 0x6c, 0x69, 0x6f, 0x6e, 0x2d,
    0x6b, 0x75, 0x77, 0x6f, 0x2d, 0x74, 0x6d, 0x65,
};

enum class State {
  kWaitForHeader = 0,
  kSeekToBody,
  kDecrypt,
};

class KuwoFileLoaderImpl : public KuwoFileLoader {
 private:
  KuwoKey key_;
  State state_ = State::kWaitForHeader;

 public:
  KuwoFileLoaderImpl(const KuwoKey& key) : key_(key) {}

  inline void InitCache() {
    u64 resource_id = ReadLittleEndian<u64>(&buf_in_[kFileKeyOffset]);
    auto rid_str = utils::Format("%" PRIu64, resource_id);
    XorBlock(key_.data(), key_.size(), rid_str.data(), rid_str.length(), 0);
  }

  inline void Decrypt(const u8* in, usize len) {
    usize pos = buf_out_.size();
    buf_out_.resize(pos + len);
    u8* p_out = &buf_out_[pos];

    XorBlock(p_out, in, len, key_.data(), key_.size(), offset_);
    offset_ += len;
  }

  bool Write(const u8* in, usize len) override {
    while (len) {
      switch (state_) {
        case State::kWaitForHeader:
          if (ReadUntilOffset(in, len, kFileHeaderSize)) {
            // Validate header.
            if (!std::equal(kKuwoMagicHeader.begin(), kKuwoMagicHeader.end(),
                            buf_in_.begin())) {
              return false;
            }

            InitCache();
            state_ = State::kSeekToBody;
          }
          break;

        case State::kSeekToBody:
          if (ReadUntilOffset(in, len, kFullHeaderSize)) {
            buf_in_.erase(buf_in_.begin(), buf_in_.begin() + kFullHeaderSize);
            state_ = State::kDecrypt;
          }
          break;

        case State::kDecrypt:
          Decrypt(in, len);
          return true;
      }
    }

    return len == 0;
  };

  bool End() override { return !InErrorState(); }
};

}  // namespace detail

std::unique_ptr<KuwoFileLoader> KuwoFileLoader::Create(const KuwoKey& key) {
  return std::make_unique<detail::KuwoFileLoaderImpl>(key);
}

}  // namespace umc::decryption::kuwo
