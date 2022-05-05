#pragma once

#include <cstdint>

#if _MSC_VER
// Windows
#include <stdlib.h>

#if BYTE_ORDER == LITTLE_ENDIAN
// We are LE.
#define umc_letoh_u32(value) (uint32_t(value))
#define umc_betoh_u32(value) (uint32_t(_byteswap_ulong(value)))
#define umc_htole_u32(value) (uint32_t(value))
#define umc_htobe_u32(value) (uint32_t(_byteswap_ulong(value)))

#define umc_letoh_u64(value) (uint64_t(value))
#define umc_betoh_u64(value) (uint64_t(_byteswap_uint64(value)))
#define umc_htole_u64(value) (uint64_t(value))
#define umc_htobe_u64(value) (uint64_t(_byteswap_uint64(value)))
#else
#define umc_letoh_u32(value) (uint32_t(_byteswap_ulong(value)))
#define umc_betoh_u32(value) (uint32_t(value))
#define umc_htole_u32(value) (uint32_t(_byteswap_ulong(value)))
#define umc_htobe_u32(value) (uint32_t(value))

#define umc_letoh_u64(value) (uint64_t(_byteswap_uint64(value)))
#define umc_betoh_u64(value) (uint64_t(value))
#define umc_htole_u64(value) (uint64_t(_byteswap_uint64(value)))
#define umc_htobe_u64(value) (uint64_t(value))
#endif

#elif defined(__APPLE__)

#include <libkern/OSByteOrder.h>

#define umc_letoh_u32(value) (uint32_t(OSSwapLittleToHostInt32(value)))
#define umc_betoh_u32(value) (uint32_t(OSSwapBigToHostInt32(value)))
#define umc_htole_u32(value) (uint32_t(OSSwapHostToLittleInt32(value)))
#define umc_htobe_u32(value) (uint32_t(OSSwapHostToBigInt32(value)))

#define umc_letoh_u64(value) (uint64_t(OSSwapLittleToHostInt64(value)))
#define umc_betoh_u64(value) (uint64_t(OSSwapBigToHostInt64(value)))
#define umc_htole_u64(value) (uint64_t(OSSwapHostToLittleInt64(value)))
#define umc_htobe_u64(value) (uint64_t(OSSwapHostToBigInt64(value)))

#else

// Linux?
#include <endian.h>

#define umc_letoh_u32(value) (uint32_t(le32toh(value)))
#define umc_betoh_u32(value) (uint32_t(be32toh(value)))
#define umc_htole_u32(value) (uint32_t(htole32(value)))
#define umc_htobe_u32(value) (uint32_t(htobe32(value)))

#define umc_letoh_u64(value) (uint64_t(le64toh(value)))
#define umc_betoh_u64(value) (uint64_t(be64toh(value)))
#define umc_htole_u64(value) (uint64_t(htole64(value)))
#define umc_htobe_u64(value) (uint64_t(htobe64(value)))

#endif

namespace umc {

inline uint32_t ReadBEU32(const void* p) {
  return umc_betoh_u32(*reinterpret_cast<const uint32_t*>(p));
}

inline uint32_t ReadLEU32(const void* p) {
  return umc_letoh_u32(*reinterpret_cast<const uint32_t*>(p));
}

inline uint64_t ReadBEU64(const void* p) {
  return umc_betoh_u64(*reinterpret_cast<const uint64_t*>(p));
}

inline uint64_t ReadLEU64(const void* p) {
  return umc_letoh_u64(*reinterpret_cast<const uint64_t*>(p));
}

}  // namespace umc
