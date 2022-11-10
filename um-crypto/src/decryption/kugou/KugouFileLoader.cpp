#include "um-crypto/decryption/kugou/KugouFileLoader.h"

#include "KGMDecryptor.h"
#include "KugouHeader.h"

#include "internal/XorHelper.h"
#include "um-crypto/endian.h"
#include "um-crypto/utils/StringHelper.h"

#include <cassert>
#include <cstring>

namespace umc::decryption::kugou {
// Private implementation

namespace detail {

constexpr std::size_t kMinimalHeaderSize = 0x2c;
typedef std::array<uint8_t, 4> KugouSlotKey1;

enum class State {
  kReadFileMagic = 0,
  kWaitForHeader,
  kSeekToBody,
  kDecrypt,
};

class KugouFileLoaderImpl : public KugouFileLoader {
 public:
  KugouFileLoaderImpl(const KGMCryptoConfig& config) : config_(config) {}

 private:
  size_t header_size_ = 0;
  std::unique_ptr<KGMCrypto> decryptor_ = nullptr;
  KGMCryptoConfig config_;
  State state_ = State::kReadFileMagic;

  inline void DecryptInput(const uint8_t*& in, std::size_t& len) {
    auto p_out = ExpandOutputBuffer(len);
    std::copy_n(in, len, p_out);

    decryptor_->Decrypt(offset_, p_out, len);

    offset_ += len;
    len = 0;
  }

  bool Write(const uint8_t* in, std::size_t len) override {
    while (len) {
      switch (state_) {
        case State::kReadFileMagic:
          if (ReadUntilOffset(in, len, sizeof(kgm_file_header))) {
            kgm_file_header header;
            memcpy(&header, buf_in_.data(), sizeof(header));
            header_size_ = header.offset_to_data;

            decryptor_ = create_kugou_decryptor(header, config_);
            if (decryptor_ == nullptr) {
              error_ = true;
              return false;
            }

            state_ = State::kSeekToBody;
          }
          break;

        case State::kSeekToBody:
          if (ReadUntilOffset(in, len, header_size_)) {
            buf_in_.erase(buf_in_.begin(), buf_in_.begin() + header_size_);
            offset_ = 0;
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

  virtual bool End() override { return true; }
};

}  // namespace detail

// Public interface

std::unique_ptr<KugouFileLoader> KugouFileLoader::Create(
    const KugouSlotKeys& slot_keys,
    const KugouV4SlotKeyExpansionTable& v4_slot_key_expansion_table,
    const KugouV4FileKeyExpansionTable& v4_file_key_expansion_table) {
  auto config = KGMCryptoConfig{
      .slot_keys = slot_keys,
      .v4_slot_key_expansion_table = v4_slot_key_expansion_table,
      .v4_file_key_expansion_table = v4_file_key_expansion_table,
  };
  return std::make_unique<detail::KugouFileLoaderImpl>(config);
}

}  // namespace umc::decryption::kugou
