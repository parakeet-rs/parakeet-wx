#pragma once
#include <cstdint>

uint8_t kgm_mask_byte_at_offset(size_t offset);
uint8_t kgm_mask_byte_at_offset_v2(size_t offset);
uint8_t kgm_decrypt_byte_at_offset_v2(uint8_t byte,
                                      uint8_t* file_key,
                                      size_t offset);
