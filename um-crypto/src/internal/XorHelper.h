#pragma once

#include "um-crypto/common.h"

namespace umc {

inline void XorBlock(u8* p_out, const u8* key, usize len) {
  for (usize i = 0; i < len; i++) {
    p_out[i] ^= key[i];
  }
}

inline void XorBlock(u8* p_out, const u8* p_in, const u8* key, usize len) {
  for (usize i = 0; i < len; i++) {
    p_out[i] = p_in[i] ^ key[i];
  }
}

inline void XorBlock(u8* p_out,
                     usize out_len,
                     const u8* key,
                     usize key_len,
                     usize offset) {
  usize j = offset % key_len;
  for (usize i = 0; i < out_len; i++) {
    p_out[i] ^= key[j++];

    if (j == key_len) {
      j = 0;
    }
  }
}

}  // namespace umc
