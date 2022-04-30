#include "um-crypto/vpr.h"
#include "kgm_data.h"
#include "um-crypto/kgm.h"

uint8_t umc::vpr::decrypt_byte_at_offset(uint8_t byte,
                                         uint8_t* file_key,
                                         size_t offset) {
  auto key = ::umc::kgm::vpr_key[offset % 17];
  return key ^ ::umc::kgm::decrypt_byte_at_offset_v2(byte, file_key, offset);
}
