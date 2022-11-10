#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <cstdlib>

#if _MSC_VER
#define __builtin_bswap64 _byteswap_uint64
#define __builtin_bswap32 _byteswap_ulong
#define __builtin_bswap16 _byteswap_ushort
#elif !defined(__has_builtin) || !__has_builtin(__builtin_bswap64)
#error "__builtin_bswap macro/methods missing."
#endif

namespace umc {

namespace detail {
constexpr bool is_le = std::endian::native == std::endian::little;
constexpr bool is_be = std::endian::native == std::endian::big;

// MSVC workaround: https://stackoverflow.com/a/36937049
#if _MSC_VER
template <class T, std::size_t... N>
constexpr T msvc_constexpr_bswap_impl(T i, std::index_sequence<N...>) {
  return ((((i >> (N * CHAR_BIT)) & (T)(unsigned char)(-1)) << ((sizeof(T) - 1 - N) * CHAR_BIT)) | ...);
};

template <class T, class U = typename std::make_unsigned<T>::type>
constexpr U msvc_constexpr_bswap(T i) {
  return msvc_constexpr_bswap_impl<U>(i, std::make_index_sequence<sizeof(T)>{});
}
#endif

template <std::integral T>
constexpr T swap_bytes(T input) {
#if _MSC_VER
  if (std::is_constant_evaluated()) {
    return msvc_constexpr_bswap(input);
  }
#endif

  if constexpr (sizeof(T) == 8) return T(__builtin_bswap64(uint64_t(input)));
  if constexpr (sizeof(T) == 4) return T(__builtin_bswap32(uint32_t(input)));
  if constexpr (sizeof(T) == 2) return T(__builtin_bswap16(uint16_t(input)));
  return input;  // uint8_t -- no conversion required
}
}  // namespace detail

////////////////////////////////////////////////////////////////////////////////
// Simple inline conversion
// LE <--> LE: noop
// BE <--> BE: noop
// Otherwise: swap

template <std::integral T>
constexpr inline T SwapHostToLittleEndian(T input) {
  if constexpr (detail::is_le) return input;
  return detail::swap_bytes(input);
}

template <std::integral T>
constexpr inline T SwapHostToBigEndian(T input) {
  if constexpr (detail::is_be) return input;
  return detail::swap_bytes(input);
}

template <std::integral T>
constexpr inline T SwapLittleEndianToHost(T input) {
  if constexpr (detail::is_le) return input;
  return detail::swap_bytes(input);
}

template <std::integral T>
constexpr inline T SwapBigEndianToHost(T input) {
  if constexpr (detail::is_be) return input;
  return detail::swap_bytes(input);
}

////////////////////////////////////////////////////////////////////////////////
// Pointer access - Read

template <std::integral A>
inline A ReadLittleEndian(const void* p) {
  return SwapLittleEndianToHost(*reinterpret_cast<const A*>(p));
}

template <std::integral A>
inline A ReadBigEndian(const void* p) {
  return SwapBigEndianToHost(*reinterpret_cast<const A*>(p));
}

////////////////////////////////////////////////////////////////////////////////
// Pointer access - Write

template <std::integral A>
inline void WriteLittleEndian(void* p, A value) {
  *reinterpret_cast<A*>(p) = SwapHostToLittleEndian(value);
}

template <std::integral A>
inline void WriteBigEndian(void* p, A value) {
  *reinterpret_cast<A*>(p) = SwapHostToBigEndian(value);
}

}  // namespace umc
