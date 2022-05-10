#pragma once

#include "../types.h"

#include <um-crypto/endian.h>
#include <um-crypto/utils/StringHelper.h>
#include <um-crypto/utils/hex.h>

#include <concepts>
#include <iomanip>
#include <type_traits>

namespace umd::utils::str_conv {

// Note:
// All "Str"s in this scope are UTF-8 encoded.

using umc::utils::Hex;
using umc::utils::HexLookup;
using umc::utils::IsHexChar;
using umc::utils::UnescapeCharSequence;
using umc::utils::Unhex;

#pragma region  // Template signature
template <typename T>
Str ToString(const T& input);

template <typename T>
void FromString(T& out, const Str& input);
#pragma endregion

// Detailed implementation
namespace detail {

#pragma region  // Str <--> Container<A> (internal)
template <std::unsigned_integral A, class TContainer>
inline Str ContainerToString(const TContainer& input) {
  if (input.size() == 0) return "";

  constexpr usize buf_single_size = sizeof(A) * 2 + 4;  // ", 0x"
  const usize buf_size = input.size() * buf_single_size;
  Str result(buf_size, 0);

  const char* fmt;
  static_assert(
      sizeof(A) == 1 || sizeof(A) == 2 || sizeof(A) == 4 || sizeof(A) == 8,
      "'A' should be one of the unsigned int.");

  if constexpr (sizeof(A) == 8) fmt = "0x%016x, ";
  else if constexpr (sizeof(A) == 4) fmt = "0x%08x, ";
  else if constexpr (sizeof(A) == 2) fmt = "0x%04x, ";
  else if constexpr (sizeof(A) == 1) fmt = "0x%02x, ";

  char* p_out = result.data();
  for (auto p_in = input.begin(); p_in < input.end(); p_in++) {
    sprintf(p_out, fmt, *p_in);
    p_out += buf_single_size;
  }
  result.erase(buf_size - 2);
  return result;
}

template <std::integral A, class TContainer>
inline void ContainerFromString(TContainer& out, const Str& input) {
  // Remember: garbage in, garbage out...

  const usize len = input.size();
  out.resize(len);

  usize i{0};
  A value{0};
  auto dst = out.begin();

  for (auto it = input.begin(); it < input.end() && dst < out.end(); it++) {
    const char c = *it;

    // ignore '0x' prefix.
    if (i == 1 && value == 0 && c == 'x') {
      i = 0;
      continue;
    }

    // skip any non-hex character.
    if (!umc::utils::IsHexChar(c)) {
      if (i > 0) {
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
  if (i > 0) *dst++ = value;

  out.resize(std::distance(out.begin(), dst));
}
#pragma endregion

}  // namespace detail

#pragma region  // Base case: String to String
template <>
inline Str ToString(const Str& input) {
  return input;
}
template <>
inline void FromString(Str& out, const Str& input) {
  out = input;
};
#pragma endregion

#pragma region  // String <--> Vec<u8>
template <>
inline Str ToString(const Vec<u8>& input) {
  return Hex(input);
}
template <>
inline void FromString(Vec<u8>& out, const Str& input) {
  out = Unhex(UnescapeCharSequence(input));
};
#pragma endregion

#pragma region  // String <--> int
template <>
inline Str ToString(const int& input) {
  std::stringstream ss;
  ss << input;
  return ss.str();
}
template <>
inline void FromString(int& out, const Str& input) {
  out = atoi(input.c_str());
}
#pragma endregion

#pragma region  // Str <--> Arr<A, Size>
template <std::unsigned_integral A, size_t Size>
inline Str ToString(const Arr<A, Size>& input) {
  return detail::ContainerToString<A>(input);
}

template <std::integral A, size_t Size>
inline void FromString(Arr<A, Size>& out, const Str& input) {
  Vec<A> result(Size);
  detail::ContainerFromString<A>(result, input);
  result.resize(Size);

  std::copy(result.begin(), result.end(), out.begin());
}
#pragma endregion

#pragma region  // Str <--> Vec<A>
template <std::unsigned_integral A>
inline Str ToString(const Vec<A>& input) {
  return detail::ContainerToString<A>(input);
}

template <std::integral A>
inline void FromString(Vec<A>& out, const Str& input) {
  detail::ContainerFromString<A>(out, input);
}
#pragma endregion

}  // namespace umd::utils::str_conv
