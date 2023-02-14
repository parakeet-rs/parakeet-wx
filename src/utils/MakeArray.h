#pragma once

#include <algorithm>
#include <memory>
#include <type_traits>

namespace parakeet_wx::utils
{

template <std::size_t SIZE, typename T = uint8_t> std::array<T, SIZE> MakeArrayFromVector(const std::vector<T> &src)
{
    std::array<T, SIZE> dst = {};
    std::copy_n(src.begin(), std::min(SIZE, src.size()), dst.begin());
    return dst;
}

} // namespace parakeet_wx::utils
