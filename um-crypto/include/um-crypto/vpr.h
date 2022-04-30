#pragma once
#include <cstddef>
#include <cstdint>

namespace umc::vpr {

uint8_t decrypt_byte_at_offset(uint8_t byte, uint8_t* file_key, size_t offset);

}  // namespace umc::vpr
