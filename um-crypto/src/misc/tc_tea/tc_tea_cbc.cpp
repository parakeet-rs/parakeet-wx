#include "tc_tea.h"

#include "um-crypto/endian.h"

#include <algorithm>

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

  Vec<u8> decrypted_v(cipher_len);
  u8* decrypted = decrypted_v.data();
  std::copy_n(cipher, cipher_len, decrypted);

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
  std::copy_n(&decrypted[start_loc], plaindata_len, plaindata);

  // Constant time zero check
  u8 zero_sum;
  for (usize i = 0; i < ZERO_LEN; i++) {
    zero_sum |= decrypted[end_loc + i];
  }

  return zero_sum == 0;
}

}  // namespace umc::misc::tc_tea::cbc
