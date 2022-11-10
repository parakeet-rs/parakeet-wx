#pragma once

#include <cstddef>
#include <cstdint>

#include <array>
#include <filesystem>
#include <string>
#include <vector>

extern const char* const umc_version;

namespace umc {

// type aliases
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

inline std::u8string U8StrFromStr(const std::string& s) {
  return std::u8string(s.begin(), s.end());
}

}  // namespace umc
