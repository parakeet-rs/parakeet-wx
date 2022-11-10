#pragma once

#include <um-crypto/endian.h>
#include <um-crypto/utils/StringHelper.h>
#include <um-crypto/utils/hex.h>

#include <concepts>
#include <iomanip>
#include <type_traits>

namespace umd::utils::str_conv {

// Note:
// All "std::string"s in this scope are UTF-8 encoded.

using umc::utils::Hex;
using umc::utils::HexLookup;
using umc::utils::IsHexChar;
using umc::utils::UnescapeCharSequence;
using umc::utils::Unhex;

#pragma region  // Template signature
template <typename T>
void ToString(std::string& out, const T& input);

template <typename T>
void FromString(T& out, const std::string& input);
#pragma endregion

// Detailed implementation
namespace detail {

#pragma region  // std::string <--> Container<A> (internal)
template <std::unsigned_integral A, class TContainer>
inline void ContainerToString(std::string& out, const TContainer& input) {
  if (input.size() == 0) {
    out = "";
    return;
  }

  constexpr std::size_t buf_single_size = sizeof(A) * 2 + 4;  // ", 0x"
  const std::size_t buf_size = input.size() * buf_single_size;
  out.resize(buf_size);

  const char* fmt;
  static_assert(sizeof(A) == 1 || sizeof(A) == 2 || sizeof(A) == 4 || sizeof(A) == 8,
                "'A' should be one of the unsigned int.");

  if constexpr (sizeof(A) == 8) fmt = "0x%016x, ";
  else if constexpr (sizeof(A) == 4) fmt = "0x%08x, ";
  else if constexpr (sizeof(A) == 2) fmt = "0x%04x, ";
  else if constexpr (sizeof(A) == 1) fmt = "0x%02x, ";

  char* p_out = out.data();
  for (auto p_in = input.begin(); p_in < input.end(); p_in++) {
    sprintf(p_out, fmt, *p_in);
    p_out += buf_single_size;
  }
  out.resize(buf_size - 2);
}

template <std::integral A, class TContainer>
inline void ContainerFromString(TContainer& out, const std::string& input) {
  // Remember: garbage in, garbage out...

  const std::size_t len = input.size();
  out.resize(len);

  std::size_t i{0};
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
inline void ToString(std::string& out, const std::string& input) {
  out = input;
}
template <>
inline void FromString(std::string& out, const std::string& input) {
  out = input;
};
#pragma endregion

#pragma region  // String <--> std::vector<uint8_t>
template <>
inline void ToString(std::string& out, const std::vector<uint8_t>& input) {
  out = Hex(input);
}
template <>
inline void FromString(std::vector<uint8_t>& out, const std::string& input) {
  out = Unhex(UnescapeCharSequence(input));
};
#pragma endregion

#pragma region  // String <--> int
template <>
inline void ToString(std::string& out, const int& input) {
  out = umc::utils::Format("%d", int(input));
}
template <>
inline void FromString(int& out, const std::string& input) {
  out = atoi(input.c_str());
}
#pragma endregion

#pragma region  // String <--> uint8_t
template <>
inline void ToString(std::string& out, const uint8_t& input) {
  out = umc::utils::Format("%d", int(input));
}
template <>
inline void FromString(uint8_t& out, const std::string& input) {
  out = atoi(input.c_str());
}
#pragma endregion

#pragma region  // std::string <--> std::array<A, Size>
template <std::unsigned_integral A, size_t Size>
inline void ToString(std::string& out, const std::array<A, Size>& input) {
  detail::ContainerToString<A>(out, input);
}

template <std::integral A, size_t Size>
inline void FromString(std::array<A, Size>& out, const std::string& input) {
  std::vector<A> result(Size);
  detail::ContainerFromString<A>(result, input);
  result.resize(Size);

  std::copy(result.begin(), result.end(), out.begin());
}
#pragma endregion

#pragma region  // std::string <--> std::vector<A>
template <std::unsigned_integral A>
inline void ToString(std::string& out, const std::vector<A>& input) {
  detail::ContainerToString<A>(out, input);
}

template <std::integral A>
inline void FromString(std::vector<A>& out, const std::string& input) {
  detail::ContainerFromString<A>(out, input);
}
#pragma endregion

}  // namespace umd::utils::str_conv
