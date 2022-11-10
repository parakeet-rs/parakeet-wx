#include "KGMDecryptor.h"
#include "um-crypto/utils/md5.h"

namespace umc::decryption::kugou {

std::array<uint8_t, 16> md5_type_3(const std::span<const uint8_t> data) {
  std::array<uint8_t, 16> result;
  auto digest = utils::md5(data);
  for (int i = 0; i < 16; i += 2) {
    result[i + 0] = digest[14 - i + 0];
    result[i + 1] = digest[14 - i + 1];
  }
  return result;
}

bool KGMCrypto3::Configure(const KGMCryptoConfig& config,
                           const std::vector<uint8_t>& slot_key,
                           const kgm_file_header& header) {
  slot_key_ = md5_type_3(slot_key);

  auto file_key = md5_type_3(std::span{header.key});
  std::copy(file_key.begin(), file_key.end(), file_key_.begin());
  file_key_[16] = 0x6b;

  return true;
}

void KGMCrypto3::Encrypt(uint64_t offset, uint8_t* buffer, size_t n) {
  uint8_t* p = buffer;
  auto slot_key_size = slot_key_.size();
  auto file_key_size = file_key_.size();

  for (size_t i = 0; i < n; i++, p++, offset++) {
    uint8_t offset_key = xor_u32_bytes(static_cast<uint32_t>(offset));
    uint8_t slot_key = slot_key_[offset % slot_key_size];
    uint8_t file_key = file_key_[offset % file_key_size];

    uint8_t temp = *p;
    temp ^= offset_key;
    temp ^= slot_key;
    temp ^= temp << 4;
    temp ^= file_key;
    *p = temp;
  }
}

void KGMCrypto3::Decrypt(uint64_t offset, uint8_t* buffer, size_t n) {
  uint8_t* p = buffer;
  auto slot_key_size = slot_key_.size();
  auto file_key_size = file_key_.size();

  for (size_t i = 0; i < n; i++, p++, offset++) {
    uint8_t offset_key = xor_u32_bytes(static_cast<uint32_t>(offset));
    uint8_t slot_key = slot_key_[offset % slot_key_size];
    uint8_t file_key = file_key_[offset % file_key_size];

    uint8_t temp = *p;
    temp ^= file_key;
    temp ^= temp << 4;
    temp ^= slot_key;
    temp ^= offset_key;
    *p = temp;
  }
}

}  // namespace umc::decryption::kugou
