#include "kgm_data.h"
#include "um-crypto/kgm.h"

uint8_t vpr_decrypt_byte_at_offset(uint8_t byte,
                                   uint8_t* file_key,
                                   size_t offset) {
  auto key = kgm_data::vpr_key[offset % 17];
  return key ^ kgm_decrypt_byte_at_offset_v2(byte, file_key, offset);
}
