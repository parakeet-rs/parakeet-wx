#pragma once
#include "../types.h"

#include <algorithm>
#include <memory>
#include <type_traits>

namespace umd::utils {

template <std::size_t SIZE, typename T = u8>
std::array<T, SIZE> MakeArrayFromVector(const std::vector<T>& src) {
  std::array<T, SIZE> dst = {};
  std::copy_n(src.begin(), std::min(SIZE, src.size()), dst.begin());
  return dst;
}

}  // namespace umd::utils
