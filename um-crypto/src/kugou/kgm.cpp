#include "um-crypto/kgm.h"
#include "kgm_data.h"

inline uint8_t xor_upper_with_lower(uint8_t result) {
  return result ^ ((result & 0x0f) << 4);
}

uint8_t umc::kgm::mask_byte_at_offset(size_t offset) {
  uint8_t result = 0;
  while (offset > 0) {
    result ^= t1[offset % table_size];
    offset >>= 4;
    result ^= t2[offset % table_size];
    offset >>= 4;
  }
  return result;
}

uint8_t umc::kgm::mask_byte_at_offset_v2(size_t offset) {
  return v2[offset % table_size] ^ umc::kgm::mask_byte_at_offset(offset >> 4);
}

uint8_t umc::kgm::decrypt_byte_at_offset_v2(uint8_t byte,
                                            uint8_t* file_key,
                                            size_t offset) {
  auto key = file_key[offset % 17];
  return xor_upper_with_lower(byte ^ key ^ mask_byte_at_offset_v2(offset));
}
