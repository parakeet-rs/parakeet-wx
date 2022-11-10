#include "um-crypto/decryption/xiami/XiamiFileLoader.h"

#include "internal/XorHelper.h"
#include "um-crypto/endian.h"
#include "um-crypto/utils/StringHelper.h"

namespace umc::decryption::xiami {

namespace detail {

constexpr std::size_t kHeaderSize = 0x10;

// 'ifmt +' \xfe *4
constexpr uint32_t kMagicHeader1 = 0x69'66'6D'74;
constexpr uint32_t kMagicHeader2 = 0xfe'fe'fe'fe;

constexpr std::size_t kMagicHeaderOffset1 = 0x00;
constexpr std::size_t kMagicHeaderOffset2 = 0x08;
constexpr std::size_t kKeyDataOffset = 0x0C;

enum class State {
  kReadHeader = 0,
  kTransparentCopy,
  kDecryptWithKey,
};

class XiamiFileLoaderImpl : public XiamiFileLoader {
 private:
  State state_ = State::kReadHeader;
  std::size_t bytes_to_copy_ = 0;
  uint8_t file_key_ = 0;

 public:
  XiamiFileLoaderImpl() {}

  bool ParseFileHeader() {
    if (ReadBigEndian<uint32_t>(&buf_in_[kMagicHeaderOffset1]) != kMagicHeader1 ||
        ReadBigEndian<uint32_t>(&buf_in_[kMagicHeaderOffset2]) != kMagicHeader2) {
      return false;
    }

    // u24_LE transparent size + uint8_t file key
    uint32_t temp = ReadLittleEndian<uint32_t>(&buf_in_[kKeyDataOffset]);
    file_key_ = (temp >> 24) - 1;
    bytes_to_copy_ = temp & 0x00'FF'FF'FF;
    return true;
  }

  bool Write(const uint8_t* in, std::size_t len) override {
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
          std::size_t copy_len = std::min(bytes_to_copy_, len);
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
          uint8_t* p_out = ExpandOutputBuffer(len);

          for (std::size_t i = 0; i < len; i++) {
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
