#include "KGMDecryptor.h"

#include "um-crypto/utils/base64.h"
#include "um-crypto/utils/hex.h"
#include "um-crypto/utils/md5.h"

#include <span>

namespace umc::decryption::kugou {

constexpr size_t V4_DIGEST_SIZE = 31;
std::array<uint8_t, V4_DIGEST_SIZE> md5_type_4(
    const std::span<const uint8_t> data) {
  static const std::array DIGEST_INDEXES = {
      0x05, 0x0e, 0x0d, 0x02, 0x0c, 0x0a, 0x0f, 0x0b, 0x03, 0x08, 0x05,
      0x06, 0x09, 0x04, 0x03, 0x07, 0x00, 0x0e, 0x0d, 0x06, 0x02, 0x0c,
      0x0a, 0x0f, 0x01, 0x0b, 0x08, 0x07, 0x09, 0x04, 0x01,
  };

  std::array<uint8_t, V4_DIGEST_SIZE> result;
  auto digest = utils::md5(data);
  for (int i = 0; i < V4_DIGEST_SIZE; i++) {
    result[i] = digest[DIGEST_INDEXES[i]];
  }
  return result;
}

std::vector<uint8_t> key_expansion_v4(const std::span<const uint8_t> table,
                                      const std::span<const uint8_t> key) {
  auto md5_final = md5_type_4(key);

  auto table_len = table.size();
  auto md5_final_len = md5_final.size();
  auto final_key_size = 4 * (md5_final_len - 1) * (table_len - 1);

  std::vector<uint8_t> expanded_key;
  expanded_key.reserve(final_key_size);
  for (uint32_t i = 1; i < static_cast<uint32_t>(md5_final_len); i++) {
    uint32_t temp1 = i * static_cast<uint32_t>(md5_final[i]);

    for (uint32_t j = 1; j < static_cast<uint32_t>(table_len); j++) {
      uint32_t temp = temp1 * j * static_cast<uint32_t>(table[j]);

      expanded_key.push_back(static_cast<uint8_t>(temp >> 0x00));
      expanded_key.push_back(static_cast<uint8_t>(temp >> 0x18));
      expanded_key.push_back(static_cast<uint8_t>(temp >> 0x10));
      expanded_key.push_back(static_cast<uint8_t>(temp >> 0x08));
    }
  }

  return expanded_key;
}

bool KGMCrypto4::Configure(const KGMCryptoConfig& config,
                           const std::vector<uint8_t>& slot_key,
                           const kgm_file_header& header) {
  // Expand slot key
  {
    auto slot_key_md5 = utils::md5(slot_key);
    auto md5_hex = umc::utils::HexCompactLowercase(slot_key_md5);
    auto md5_b64 = umc::utils::Base64EncodeBytes(
        std::span{reinterpret_cast<uint8_t*>(md5_hex.data()), md5_hex.size()});
    slot_key_ = key_expansion_v4(config.v4_slot_key_expansion_table, md5_b64);
  }

  // Expand file key
  {
    file_key_ = key_expansion_v4(config.v4_file_key_expansion_table,
                                 std::span{header.key});
  }

  return true;
}

void KGMCrypto4::Encrypt(uint64_t offset, uint8_t* buffer, size_t n) {
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

void KGMCrypto4::Decrypt(uint64_t offset, uint8_t* buffer, size_t n) {
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
