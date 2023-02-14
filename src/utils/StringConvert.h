#pragma once

#include <array>
#include <iomanip>
#include <sstream>
#include <type_traits>
#include <vector>

#include "HexHelper.h"

namespace parakeet_wx::utils::str_conv
{

// Note:
// All "std::string"s in this scope are UTF-8 encoded.

#pragma region // Template signature
template <typename T> void ToString(std::string &out, const T &input);

template <typename T> void FromString(T &out, const std::string &input);
#pragma endregion

// Detailed implementation
namespace detail
{
#pragma region // std::string <--> Container<A> (internal)
template <typename A, class Container, typename = std::enable_if_t<std::is_unsigned_v<A> && std::is_integral_v<A>>>
inline void ContainerToString(std::string &out, const Container &input)
{
    if (input.size() == 0)
    {
        out = "";
        return;
    }

    constexpr std::size_t buf_single_size = sizeof(A) * 2 + 4; // ", 0x"
    const std::size_t buf_size = input.size() * buf_single_size;
    out.resize(buf_size);

    const char *fmt;
    static_assert(sizeof(A) == 1 || sizeof(A) == 2 || sizeof(A) == 4 || sizeof(A) == 8,
                  "'A' should be one of the unsigned int.");

    if constexpr (sizeof(A) == 8)
        fmt = "0x%016x, ";
    else if constexpr (sizeof(A) == 4)
        fmt = "0x%08x, ";
    else if constexpr (sizeof(A) == 2)
        fmt = "0x%04x, ";
    else if constexpr (sizeof(A) == 1)
        fmt = "0x%02x, ";

    char *p_out = out.data();
    for (auto p_in = input.begin(); p_in < input.end(); p_in++)
    {
        sprintf(p_out, fmt, *p_in);
        p_out += buf_single_size;
    }
    out.resize(buf_size - 2);
}

template <typename A, class Container, typename = std::enable_if_t<std::is_unsigned_v<A> && std::is_integral_v<A>>>
inline void ContainerFromString(Container &out, const std::string &input)
{
    // Remember: garbage in, garbage out...
    const std::size_t len = input.size();
    out.resize(len);

    std::size_t i{0};
    A value{0};
    auto dst = out.begin();

    for (auto it = input.begin(); it < input.end() && dst < out.end(); it++)
    {
        const char c = *it;

        // ignore '0x' prefix.
        if (i == 1 && value == 0 && c == 'x')
        {
            i = 0;
            continue;
        }

        // skip any non-hex character.
        if (!IsHexChar(c))
        {
            if (i > 0)
            {
                *dst++ = value;
                i = 0;
                value = 0;
            }

            continue;
        }

        i++;
        value = HexLookup(c) | (value << 4);
    }

    // Add last item
    if (i > 0)
        *dst++ = value;

    out.resize(std::distance(out.begin(), dst));
}
#pragma endregion

} // namespace detail

#pragma region // Base case: String to String
template <> inline void ToString(std::string &out, const std::string &input)
{
    out = input;
}
template <> inline void FromString(std::string &out, const std::string &input)
{
    out = input;
};
#pragma endregion

#pragma region // String <--> int
template <> inline void ToString(std::string &out, const int &input)
{
    std::stringstream ss;
    ss << input;
    out = ss.str();
}
template <> inline void FromString(int &out, const std::string &input)
{
    out = atoi(input.c_str());
}
#pragma endregion

#pragma region // String <--> uint8_t
template <> inline void ToString(std::string &out, const uint8_t &input)
{
    std::stringstream ss;
    ss << int(input);
    out = ss.str();
}
template <> inline void FromString(uint8_t &out, const std::string &input)
{
    out = atoi(input.c_str());
}
#pragma endregion

#pragma region // std::string <--> std::array<A, Size>
template <typename A, size_t Size> inline void ToString(std::string &out, const std::array<A, Size> &input)
{
    detail::ContainerToString<A>(out, input);
}

template <typename A, size_t Size> inline void FromString(std::array<A, Size> &out, const std::string &input)
{
    std::vector<A> result(Size);
    detail::ContainerFromString<A>(result, input);
    result.resize(Size);

    std::copy(result.begin(), result.end(), out.begin());
}
#pragma endregion

#pragma region // std::string <--> std::vector<A>
template <typename A> inline void ToString(std::string &out, const std::vector<A> &input)
{
    detail::ContainerToString<A>(out, input);
}

template <typename A> inline void FromString(std::vector<A> &out, const std::string &input)
{
    detail::ContainerFromString<A>(out, input);
}
#pragma endregion

} // namespace parakeet_wx::utils::str_conv
