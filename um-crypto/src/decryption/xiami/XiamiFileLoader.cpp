#include "um-crypto/decryption/xiami/XiamiFileLoader.h"

#include "internal/XorHelper.h"
#include "um-crypto/endian.h"
#include "um-crypto/utils/StringHelper.h"

namespace umc::decryption::xiami {

namespace detail {

constexpr usize kHeaderSize = 0x10;

// 'ifmt +' \xfe *4
constexpr u32 kMagicHeader1 = 0x69'66'6D'74;
constexpr u32 kMagicHeader2 = 0xfe'fe'fe'fe;

constexpr usize kMagicHeaderOffset1 = 0x00;
constexpr usize kMagicHeaderOffset2 = 0x08;
constexpr usize kKeyDataOffset = 0x0C;

enum class State {
  kReadHeader = 0,
  kTransparentCopy,
  kDecryptWithKey,
};

class XiamiFileLoaderImpl : public XiamiFileLoader {
 private:
  State state_ = State::kReadHeader;
  usize bytes_to_copy_ = 0;
  u8 file_key_ = 0;

 public:
  XiamiFileLoaderImpl() {}

  bool ParseFileHeader() {
    if (ReadBigEndian<u32>(&buf_in_[kMagicHeaderOffset1]) != kMagicHeader1 ||
        ReadBigEndian<u32>(&buf_in_[kMagicHeaderOffset2]) != kMagicHeader2) {
      return false;
    }

    // u24_LE transparent size + u8 file key
    u32 temp = ReadLittleEndian<u32>(&buf_in_[kKeyDataOffset]);
    file_key_ = (temp >> 24) - 1;
    bytes_to_copy_ = temp & 0x00'FF'FF'FF;
    return true;
  }

  bool Write(const u8* in, usize len) override {
    while (len) {
      switch (state_) {
        case State::kReadHeader:
          if (ReadUntilOffset(in, len, kHeaderSize)) {
            if (!ParseFileHeader()) {
              error_ = "file header magic not found";
              return false;
            }
            buf_in_.erase(buf_in_.begin(), buf_in_.begin() + kHeaderSize);
            state_ = State::kTransparentCopy;
          }
          break;

        case State::kTransparentCopy: {
          usize copy_len = std::min(bytes_to_copy_, len);
          buf_out_.insert(buf_out_.end(), in, in + copy_len);

          in += copy_len;
          len -= copy_len;
          bytes_to_copy_ -= copy_len;
          offset_ += copy_len;

          if (bytes_to_copy_ == 0) {
            state_ = State::kDecryptWithKey;
          }
          break;
        }

        case State::kDecryptWithKey: {
          u8* p_out = ExpandOutputBuffer(len);

          for (usize i = 0; i < len; i++) {
            p_out[i] = file_key_ - in[i];
          }

          return true;
        }
      }
    }

    return len == 0;
  };

  bool End() override { return !InErrorState(); }
};

}  // namespace detail

std::unique_ptr<XiamiFileLoader> XiamiFileLoader::Create() {
  return std::make_unique<detail::XiamiFileLoaderImpl>();
}

}  // namespace umc::decryption::xiami
