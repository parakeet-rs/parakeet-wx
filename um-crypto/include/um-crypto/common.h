#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

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

using usize = size_t;
#if UINTPTR_MAX == UINT64_MAX
using isize = i64;
#else
using isize = i32;
#endif

template <class T>
using Vec = std::vector<T>;

using Str = std::string;

}  // namespace umc
