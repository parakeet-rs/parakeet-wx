#include "um-crypto/decryption/netease/NCMFileLoader.h"
#include "um-crypto/endian.h"
#include "um-crypto/utils/StringHelper.h"

#include "internal/XorHelper.h"

#include "cryptopp/aes.h"
#include "cryptopp/filters.h"
#include "cryptopp/modes.h"

namespace umc::decryption::netease {

namespace detail {

/**
 * @brief NCM file format
 *
 * File header: Hardcoded 8 char + 2 padding (hardcoded?)
 * 0000h: 43 54 45 4E 46 44 41 4D 01 69  CTENFDAM.i
 *
 * Followed by 3 blocks:
 *   - Content Key (Encrypted using `NCMContentKeyProtectionKey`)
 *   - Metadata; (Encrypted, ignored by this library)
 *   - Album Cover (prefixed with 5 bytes padding? ignored by this library);
 *   - Audio Data (Encrypted with Content Key);
 */

constexpr usize kFileHeaderSize = 10;  // 'CTENFDAM'
constexpr u64 kNCMFileMagic = 0x43'54'45'4E'46'44'41'4D;

// "neteasecloudmusic"
Arr<u8, 17> kContentKeyPrefix = {
    'n', 'e', 't', 'e', 'a', 's', 'e', 'c', 'l',
    'o', 'u', 'd', 'm', 'u', 's', 'i', 'c',
};

enum class State {
  kReadFileMagic = 0,

  kParseFileKey,
  kReadMetaBlock,
  kReadCoverFrameSize,
  kReadCoverBlock,
  kSkipCoverPadding,
  kDecryptAudio
};

class NCMFileLoaderImpl : public NCMFileLoader {
 private:
  State state_ = State::kReadFileMagic;
  NCMContentKeyProtectionKey key_;
  Vec<u8> content_key_;

  u32 content_key_size_ = 0;
  u32 metadata_size_ = 0;
  u32 cover_frame_size_ = 0;
  u32 cover_size_ = 0;

 public:
  NCMFileLoaderImpl(const NCMContentKeyProtectionKey& key) : key_(key) {}

  bool ParseFileKey() {
    using AES = CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption;
    using Filter = CryptoPP::StreamTransformationFilter;

    Vec<u8> file_key(content_key_size_);
    ConsumeInput(file_key.data(), content_key_size_);
    for (auto& key : file_key) {
      key ^= 0x64;
    }

    try {
      AES aes(key_.data(), key_.size());
      Filter decryptor(aes, nullptr, Filter::PKCS_PADDING);
      decryptor.PutMessageEnd(file_key.data(), file_key.size());
      content_key_.resize(decryptor.MaxRetrievable());
      decryptor.Get(content_key_.data(), content_key_.size());
    } catch (const std::exception& ex) {
      error_ = utils::Format("could not decrypt content key: ", ex.what());
      return false;
    }

    if (!std::equal(kContentKeyPrefix.begin(), kContentKeyPrefix.end(),
                    content_key_.begin())) {
      error_ = "invalid key prefix";
      return false;
    }

    content_key_.erase(content_key_.begin(),
                       content_key_.begin() + kContentKeyPrefix.size());
    return true;
  }

  usize audio_data_offset_ = 0;
  Arr<u8, 0x100> final_audio_xor_key_;
  void InitXorCipherKey() {
    u8 S[0x100];

    /* Standard RC4 setup */ {
      auto& key = content_key_;
      usize key_len = key.size();

      for (usize i = 0; i <= 0xff; i++) {
        S[i] = u8(i);
      }

      u8 j = 0;
      for (usize i = 0; i <= 0xff; i++) {
        j += S[i] + key[i % key_len];
        std::swap(S[i], S[j]);
      }
    }

    /* RC4(NCM variant) - Key Derivation */ {
      // Looks like an non-standard RC4 PRNG derivation?
      // Was this done on purpose, to compensate for the fact that RC4 can't
      //   seek?

      u8 i = 0;
      auto derive_next_byte = [&S, &i]() -> u8 {
        i++;
        u8 j = S[i] + i;  // In a standard RC4, this would be `j += S[i]`
                          //   followed by `swap(S[i], S[j])`
        return S[u8(S[i] + S[j])];
      };

      // Derive some keys...
      for (auto& k : final_audio_xor_key_) {
        k = derive_next_byte();
      }
    }
  }

  bool ReadNextSizedBlock(const u8*& in,
                          usize& len,
                          u32& next_block_size,
                          usize padding = 0) {
    if (InErrorState()) return false;

    if (next_block_size == 0 && ReadBlock(in, len, sizeof(u32))) {
      ConsumeInput(&next_block_size, sizeof(u32));
      next_block_size = SwapLittleEndianToHost(next_block_size) + padding;

      if (next_block_size == 0) {
        error_ = "file key size = 0";
        return false;
      }
    }

    if (next_block_size > 0 && ReadBlock(in, len, next_block_size)) {
      return true;
    }

    return false;
  }

  bool Write(const u8* in, usize len) override {
    while (len) {
      switch (state_) {
        case State::kReadFileMagic:
          if (ReadUntilOffset(in, len, kFileHeaderSize)) {
            if (ReadBigEndian<u64>(buf_in_.data()) != kNCMFileMagic) {
              error_ = "not a valid ncm file";
              return false;
            }

            ConsumeInput(kFileHeaderSize);
            state_ = State::kParseFileKey;
          }
          break;

        case State::kParseFileKey:
          if (ReadNextSizedBlock(in, len, content_key_size_)) {
            if (!ParseFileKey()) {
              return false;
            }
            state_ = State::kReadMetaBlock;
          }
          break;

        case State::kReadMetaBlock:
          // unknown 5 bytes padding;
          if (ReadNextSizedBlock(in, len, metadata_size_, 5)) {
            ConsumeInput(usize{metadata_size_});
            state_ = State::kReadCoverFrameSize;
          }
          break;

        case State::kReadCoverFrameSize:
          if (ReadBlock(in, len, sizeof(cover_frame_size_))) {
            ConsumeInput(&cover_frame_size_, sizeof(cover_frame_size_));
            cover_frame_size_ = SwapLittleEndianToHost(cover_frame_size_);
            state_ = State::kReadCoverBlock;
          }
          break;

        case State::kReadCoverBlock:
          if (ReadNextSizedBlock(in, len, cover_size_)) {
            if (cover_frame_size_ < cover_size_) {
              error_ = "cover frame size is smaller than cover size.";
              return false;
            }

            ConsumeInput(usize{cover_size_});
            InitXorCipherKey();
            audio_data_offset_ = 0;

            state_ = State::kSkipCoverPadding;
          }
          break;

        case State::kSkipCoverPadding:
          if (ReadBlock(in, len, cover_frame_size_ - cover_size_)) {
            ConsumeInput(usize{cover_frame_size_ - cover_size_});
            state_ = State::kDecryptAudio;
          }
          break;

        case State::kDecryptAudio: {
          auto p_out = ExpandOutputBuffer(len);
          XorBlock(p_out, in, len, final_audio_xor_key_.data(),
                   final_audio_xor_key_.size(), audio_data_offset_);
          offset_ += len;
          audio_data_offset_ += len;
          return true;
        }
      }
    }

    return len == 0;
  };

  bool End() override { return !InErrorState(); };
};

}  // namespace detail

std::unique_ptr<NCMFileLoader> NCMFileLoader::Create(
    const NCMContentKeyProtectionKey& key) {
  return std::make_unique<detail::NCMFileLoaderImpl>(key);
}

}  // namespace umc::decryption::netease
