#pragma once

#include <climits>
#include <cstdint>
#include <cstdlib>
#include <type_traits>

namespace parakeet_wx::utils
{

// NOLINTBEGIN(*-type-reinterpret-cast)

namespace detail
{

#if _MSC_VER
#ifndef __builtin_bswap64
#define __PARAKEET_CLEAR_BUILTIN 1
#define __builtin_bswap64 _byteswap_uint64
#define __builtin_bswap32 _byteswap_ulong
#define __builtin_bswap16 _byteswap_ushort
#endif // #ifndef __builtin_bswap64

#elif (__clang__ || __GNUG__)
// OK - clang / g++ should have this already.
#elif !defined(__has_builtin) || !__has_builtin(__builtin_bswap64)
#error __builtin_bswap_xx macro/methods missing.
#endif

template <typename T> constexpr T swap_bytes(T input)
{
    static_assert(std::is_integral_v<T>, "T should be a integral type.");

    if constexpr (std::is_same_v<std::make_unsigned_t<T>, uint64_t>)
    {
        return static_cast<T>(__builtin_bswap64(static_cast<uint64_t>(input)));
    }
    else if constexpr (std::is_same_v<std::make_unsigned_t<T>, uint32_t>)
    {
        return static_cast<T>(__builtin_bswap32(static_cast<uint32_t>(input)));
    }
    else if constexpr (std::is_same_v<std::make_unsigned_t<T>, uint16_t>)
    {
        return static_cast<T>(__builtin_bswap16(static_cast<uint16_t>(input)));
    }
    else
    {
        static_assert(std::is_same_v<std::make_unsigned_t<T>, uint8_t>, "unsupported type for swap_bytes");
        return input; // uint8_t -- no conversion required
    }
}

#if __PARAKEET_CLEAR_BUILTIN
#undef __builtin_bswap64
#undef __builtin_bswap32
#undef __builtin_bswap16
#endif

class Endian
{
  private:
    static constexpr uint32_t uint32_ = 0x01020304;
    static constexpr uint8_t magic_ = (const uint8_t &)uint32_;

  public:
    Endian() = delete;
    static constexpr bool little = magic_ == 0x04;
    static constexpr bool middle = magic_ == 0x02;
    static constexpr bool big = magic_ == 0x01;
    static_assert(little || middle || big, "Cannot determine endianness!");
};

} // namespace detail

////////////////////////////////////////////////////////////////////////////////
// Simple inline conversion
// LE <--> LE: noop
// BE <--> BE: noop
// Otherwise: swap

template <typename T> constexpr T SwapHostToLittleEndian(T input)
{
    if constexpr (detail::Endian::little)
    {
        return input;
    }
    else
    {
        return detail::swap_bytes(input);
    }
}

template <typename T> constexpr T SwapHostToBigEndian(T input)
{
    if constexpr (detail::Endian::big)
    {
        return input;
    }
    else
    {
        return detail::swap_bytes(input);
    }
}

template <typename T> constexpr T SwapLittleEndianToHost(T input)
{
    if constexpr (detail::Endian::little)
    {
        return input;
    }
    else
    {
        return detail::swap_bytes(input);
    }
}

template <typename T> constexpr T SwapBigEndianToHost(T input)
{
    if constexpr (detail::Endian::big)
    {
        return input;
    }
    else
    {
        return detail::swap_bytes(input);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Pointer access - Read

template <typename A> inline A ReadBigEndian(const uint8_t *ptr)
{
    return SwapHostToBigEndian(*reinterpret_cast<const A *>(ptr));
}

template <typename A> inline A ReadLittleEndian(const uint8_t *ptr)
{
    return SwapHostToLittleEndian(*reinterpret_cast<const A *>(ptr));
}

////////////////////////////////////////////////////////////////////////////////
// Pointer access - Write

template <typename A> inline void WriteLittleEndian(uint8_t *ptr, A value)
{
    *reinterpret_cast<A *>(ptr) = SwapHostToLittleEndian(value);
}

template <typename A> inline void WriteBigEndian(uint8_t *ptr, A value)
{
    *reinterpret_cast<A *>(ptr) = SwapHostToBigEndian(value);
}

// NOLINTEND(*-type-reinterpret-cast)

} // namespace parakeet_wx::utils
