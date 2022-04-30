#pragma once
#include <cstdint>

namespace umc::kgm {

uint8_t mask_byte_at_offset(size_t offset);
uint8_t mask_byte_at_offset_v2(size_t offset);
uint8_t decrypt_byte_at_offset_v2(uint8_t byte,
                                  uint8_t* file_key,
                                  size_t offset);

}  // namespace umc::kgm
