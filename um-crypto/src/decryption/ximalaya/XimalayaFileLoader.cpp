#include "um-crypto/decryption/ximalaya/XimalayaFileLoader.h"

#include "internal/XorHelper.h"
#include "um-crypto/utils/StringHelper.h"

namespace umc::decryption::ximalaya {

namespace detail {

constexpr std::size_t kEncryptedHeaderSize = 0x400;

enum class State {
  kDecryptHeader = 0,
  kPassthrough,
};

template <std::size_t ContentKeySize>
class XimalayaFileLoaderImpl : public XimalayaFileLoader {
 private:
  std::string name_;
  std::array<u8, ContentKeySize> content_key_;
  ScrambleTable scramble_table_;
  State state_ = State::kDecryptHeader;

 public:
  XimalayaFileLoaderImpl(const std::array<u8, ContentKeySize>& content_key,
                         const ScrambleTable& scramble_table,
                         const char* subtype)
      : content_key_(content_key), scramble_table_(scramble_table) {
    name_ = utils::Format("Ximalaya(%s)", subtype);
  }

  virtual const std::string GetName() const override { return name_; };

  void DoHeaderDecryption() {
    auto p_out = ExpandOutputBuffer(kEncryptedHeaderSize);

    for (std::size_t i = 0; i < kEncryptedHeaderSize; i++) {
      std::size_t idx = scramble_table_[i];
      p_out[i] = buf_in_[idx] ^ content_key_[i % ContentKeySize];
    }
  }

  bool Write(const u8* in, std::size_t len) override {
    while (len) {
      switch (state_) {
        case State::kDecryptHeader:
          if (ReadUntilOffset(in, len, kEncryptedHeaderSize)) {
            DoHeaderDecryption();
            state_ = State::kPassthrough;
          }
          break;

        case State::kPassthrough:
          buf_out_.insert(buf_out_.end(), in, in + len);
          return true;
      }
    }

    return len == 0;
  };

  bool End() override { return !InErrorState(); }
};

}  // namespace detail

std::unique_ptr<XimalayaFileLoader> XimalayaFileLoader::Create(
    const X2MContentKey& key,
    const ScrambleTable& scramble_table) {
  X3MContentKey key_x2m;
  static_assert(
      (kX3MContentKeySize % kX2MContentKeySize) == 0,
      "X3M content key size should be multiple of X2M content key size");

  for (std::size_t i = 0; i < kX3MContentKeySize; i += kX2MContentKeySize) {
    std::copy(key.begin(), key.end(), &key_x2m[i]);
  }

  return std::make_unique<detail::XimalayaFileLoaderImpl<kX3MContentKeySize>>(
      key_x2m, scramble_table, "X2M");
}

std::unique_ptr<XimalayaFileLoader> XimalayaFileLoader::Create(
    const X3MContentKey& key,
    const ScrambleTable& scramble_table) {
  return std::make_unique<detail::XimalayaFileLoaderImpl<kX3MContentKeySize>>(
      key, scramble_table, "X3M");
}

}  // namespace umc::decryption::ximalaya
