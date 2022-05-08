#pragma once
#include "../types.h"

#include <algorithm>
#include <memory>
#include <type_traits>

namespace umd::utils {

template <usize SIZE, typename T = u8>
Arr<T, SIZE> MakeArrayFromVector(const Vec<T>& src) {
  Arr<T, SIZE> dst = {};
  std::copy_n(src.begin(), std::min(SIZE, src.size()), dst.begin());
  return dst;
}

}  // namespace umd::utils
