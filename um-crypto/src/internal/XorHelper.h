#pragma once

#include "um-crypto/types.h"

namespace umc {

/**
 * @brief XOR operation of two blocks.
 *
 * @param p_in_out
 * @param p_key
 * @param len
 */
inline void XorBlock(void* p_in_out, const void* p_key, usize len) {
  for (usize i = 0; i < len; i++) {
    reinterpret_cast<u8*>(p_in_out)[i] ^= reinterpret_cast<const u8*>(p_key)[i];
  }
}

template <std::integral T>
inline void XorInt(void* p_in_out, const void* p_key) {
  *reinterpret_cast<T*>(p_in_out) ^= *reinterpret_cast<const T*>(p_key);
}

/**
 * @brief XOR operation of two blocks (in1 & in2),
 *        then store its result to p_out.
 *
 * @param p_out
 * @param p_in1
 * @param p_in2
 * @param len
 */
inline void XorBlock(void* p_out,
                     const void* p_in1,
                     const void* p_in2,
                     usize len) {
  for (usize i = 0; i < len; i++) {
    reinterpret_cast<u8*>(p_out)[i] = reinterpret_cast<const u8*>(p_in1)[i] ^
                                      reinterpret_cast<const u8*>(p_in2)[i];
  }
}

/**
 * @brief XOR operation of a data block and key block.
 *        Once key offset had reached the end, it will reset to index 0.
 *
 * @param p_in_out
 * @param out_len
 * @param key
 * @param key_len
 * @param key_offset
 */
inline void XorBlock(void* p_in_out,
                     usize out_len,
                     const void* key,
                     usize key_len,
                     usize key_offset) {
  const usize j = key_offset % key_len;
  for (usize i = 0; i < out_len; i++) {
    reinterpret_cast<u8*>(p_in_out)[i] ^=
        reinterpret_cast<const u8*>(key)[(j + i) % key_len];
  }
}

/**
 * @brief XOR operation of a data block and key block, then stored in p_out.
 *        Once key offset had reached the end, it will reset to index 0.
 *
 * @param p_out
 * @param p_in1
 * @param int1_len
 * @param key
 * @param key_len
 * @param key_offset
 */
inline void XorBlock(void* p_out,
                     const void* p_in1,
                     usize len,
                     const void* key,
                     usize key_len,
                     usize key_offset) {
  const usize j = key_offset % key_len;
  for (usize i = 0; i < len; i++) {
    reinterpret_cast<u8*>(p_out)[i] =
        reinterpret_cast<const u8*>(p_in1)[i] ^
        reinterpret_cast<const u8*>(key)[(j + i) % key_len];
  }
}

}  // namespace umc
