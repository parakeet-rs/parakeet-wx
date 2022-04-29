#include "um-crypto/kgm.h"
#include "kgm_data.h"

inline uint8_t xor_upper_with_lower(uint8_t result) {
  return result ^ ((result & 0x0f) << 4);
}

uint8_t kgm_mask_byte_at_offset(size_t offset) {
  uint8_t result = 0;
  while (offset > 0) {
    result ^= kgm_data::t1[offset % kgm_data::table_size];
    offset >>= 4;
    result ^= kgm_data::t2[offset % kgm_data::table_size];
    offset >>= 4;
  }
  return result;
}

uint8_t kgm_mask_byte_at_offset_v2(size_t offset) {
  return kgm_data::v2[offset % kgm_data::table_size] ^
         kgm_mask_byte_at_offset(offset >> 4);
}

uint8_t kgm_decrypt_byte_at_offset_v2(uint8_t byte,
                                      uint8_t* file_key,
                                      size_t offset) {
  auto key = file_key[offset % 17];
  return xor_upper_with_lower(byte ^ key ^ kgm_mask_byte_at_offset_v2(offset));
}
