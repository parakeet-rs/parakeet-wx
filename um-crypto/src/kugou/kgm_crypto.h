#pragma once
#include "kgm_data.h"
#include "um-crypto/common.h"

namespace umc::kugou {

inline uint8_t GetMaskV1(size_t offset) {
  uint8_t result = 0;
  while (offset > 0) {
    result ^= t1[offset % table_size];
    offset >>= 4;
    result ^= t2[offset % table_size];
    offset >>= 4;
  }
  return result;
}

inline uint8_t GetMaskV2(size_t offset) {
  return v2[offset % table_size] ^ GetMaskV1(offset >> 4);
}

inline u8 ScrambleByte(u8 result) {
  return result ^ ((result & 0x0f) << 4);
}

inline u8 DecryptKGMV2(usize offset, u8 input, const u8* file_key) {
  const u8 byte = input ^ file_key[offset % file_key_size] ^ GetMaskV2(offset);
  return ScrambleByte(byte);
}

inline u8 DecryptVPRV2(usize offset, u8 input, const u8* file_key) {
  return DecryptKGMV2(offset, input, file_key) ^ vpr_key[offset % vpr_key_size];
}

}  // namespace umc::kugou
