#include "KGMDecryptor.h"

namespace umc::decryption::kugou {

bool KGMCrypto2::Configure(const KGMCryptoConfig& config,
                           const std::vector<uint8_t>& slot_key,
                           const kgm_file_header& header) {
  key_ = slot_key;
  return true;
}

void KGMCrypto2::Encrypt(uint64_t offset, uint8_t* buffer, size_t n) {
  uint8_t* p = buffer;
  for (size_t i = 0; i < n; i++, p++, offset++) {
    uint8_t key = key_[offset % key_.size()];

    uint8_t temp = buffer[i];
    temp ^= key;
    temp ^= temp << 4;
    buffer[i] = temp;
  }
}

void KGMCrypto2::Decrypt(uint64_t offset, uint8_t* buffer, size_t n) {
  uint8_t* p = buffer;
  for (size_t i = 0; i < n; i++, p++, offset++) {
    uint8_t key = key_[offset % key_.size()];

    uint8_t temp = *p;
    temp ^= temp << 4;
    temp ^= key;
    *p = temp;
  }
}

}  // namespace umc::decryption::kugou
