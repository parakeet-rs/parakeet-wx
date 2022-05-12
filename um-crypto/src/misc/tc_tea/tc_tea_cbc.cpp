#include "tc_tea.h"

#include "um-crypto/endian.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace umc::misc::tc_tea::ecb {

constexpr u32 ROUNDS = 16;
constexpr u32 DELTA = 0x9e3779b9;

inline u32 single_round_arithmetic(u32 value, u32 sum, u32 key1, u32 key2) {
  return ((value << 4) + key1) ^ (value + sum) ^ ((value >> 5) + key2);
}

void DecryptBlock(void* block, u32* k) {
  auto block_u32 = reinterpret_cast<u32*>(block);

  u32 y = SwapBigEndianToHost(block_u32[0]);
  u32 z = SwapBigEndianToHost(block_u32[1]);
  u32 sum = DELTA * ROUNDS;

  for (int i = 0; i < ROUNDS; i++) {
    z -= single_round_arithmetic(y, sum, k[2], k[3]);
    y -= single_round_arithmetic(z, sum, k[0], k[1]);
    sum -= DELTA;
  }

  block_u32[0] = SwapHostToBigEndian(y);
  block_u32[1] = SwapHostToBigEndian(z);
}

}  // namespace umc::misc::tc_tea::ecb

namespace umc::misc::tc_tea::cbc {

constexpr usize SALT_LEN = 2;
constexpr usize ZERO_LEN = 7;
constexpr usize FIXED_PADDING_LEN = 1 + SALT_LEN + ZERO_LEN;

void ParseBigEndianKey(u32* result, const u8* key) {
  auto key_u32_be = reinterpret_cast<const u32*>(key);

  for (int i = 0; i < 4; i++) {
    result[i] = SwapBigEndianToHost(key_u32_be[i]);
  }
}

bool Decrypt(u8* plaindata,
             usize& plaindata_len,
             const u8* cipher,
             usize cipher_len,
             const u8* key) {
  plaindata_len = 0;
  u32 k[4];
  ParseBigEndianKey(k, key);

  if (cipher_len < FIXED_PADDING_LEN || cipher_len % 8 != 0) {
    return false;
  }

  u8* decrypted = static_cast<u8*>(malloc(cipher_len));
  memcpy(decrypted, cipher, cipher_len);

  // decrypt first block
  ecb::DecryptBlock(decrypted, k);
  for (usize i = 8; i < cipher_len; i += 8) {
    // xor with previous block first
    for (int j = i - 8; j < i; j++) {
      decrypted[j + 8] ^= decrypted[j];
    }

    ecb::DecryptBlock(&decrypted[i], k);
  }

  for (usize i = 8; i < cipher_len; i++) {
    decrypted[i] ^= cipher[i - 8];
  }

  usize pad_size = usize(decrypted[0] & 0b111);
  usize start_loc = 1 + pad_size + SALT_LEN;
  usize end_loc = cipher_len - ZERO_LEN;

  plaindata_len = end_loc - start_loc;
  memcpy(plaindata, &decrypted[start_loc], plaindata_len);

  free(decrypted);

  return true;
}

}  // namespace umc::misc::tc_tea::cbc
