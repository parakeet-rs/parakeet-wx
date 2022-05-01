#pragma once
#include "endian.h"

#include <cstdint>

namespace umc {

inline uint32_t ReadBEU32(const void* p) {
  return umc_betoh_u32(*reinterpret_cast<const uint32_t*>(p));
}

inline uint32_t ReadLEU32(const void* p) {
  return umc_letoh_u32(*reinterpret_cast<const uint32_t*>(p));
}

}  // namespace umc
