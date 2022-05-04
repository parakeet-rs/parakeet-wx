#pragma once

#include <cstdint>

#if _MSC_VER
// Windows
#include <stdlib.h>

#if BYTE_ORDER == LITTLE_ENDIAN
// We are LE.
#define umc_letoh_u32(value) (value)
#define umc_betoh_u32(value) (_byteswap_ulong(value))
#define umc_htole_u32(value) (value)
#define umc_htobe_u32(value) (_byteswap_ulong(value))
#else
#define umc_letoh_u32(value) (_byteswap_ulong(value))
#define umc_betoh_u32(value) (value)
#define umc_htole_u32(value) (_byteswap_ulong(value))
#define umc_htobe_u32(value) (value)
#endif

#elif defined(__APPLE__)

#include <libkern/OSByteOrder.h>

#define umc_letoh_u32(value) (OSSwapLittleToHostInt32(value))
#define umc_betoh_u32(value) (OSSwapBigToHostInt32(value))
#define umc_htole_u32(value) (OSSwapHostToLittleInt32(value))
#define umc_htobe_u32(value) (OSSwapHostToBigInt32(value))

#else

// Linux?
#include <endian.h>

#define umc_letoh_u32(value) (le32toh(value))
#define umc_betoh_u32(value) (be32toh(value))
#define umc_htole_u32(value) (htole32(value))
#define umc_htobe_u32(value) (htobe32(value))

#endif

namespace umc {

inline uint32_t ReadBEU32(const void* p) {
  return umc_betoh_u32(*reinterpret_cast<const uint32_t*>(p));
}

inline uint32_t ReadLEU32(const void* p) {
  return umc_letoh_u32(*reinterpret_cast<const uint32_t*>(p));
}

}  // namespace umc
