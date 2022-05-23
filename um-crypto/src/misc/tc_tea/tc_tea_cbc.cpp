#include "tc_tea.h"

#include "internal/XorHelper.h"
#include "um-crypto/endian.h"

#include <algorithm>
#include <random>

namespace umc::misc::tc_tea::cbc {

using independent_u8_engine =
    std::independent_bits_engine<std::default_random_engine,
                                 std::numeric_limits<u8>::digits,
                                 u16>;

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
    XorInt<u64>(&decrypted[i], &decrypted[i - 8]);
    ecb::DecryptBlock(&decrypted[i], k);
  }

  // Hint compiler that we are XOR block of 8.
  for (usize i = 8; i < cipher_len; i += 8) {
    XorInt<u64>(&decrypted[i], &cipher[i - 8]);
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

usize GetEncryptedSize(usize size) {
  usize len = FIXED_PADDING_LEN + size;
  usize pad_len = (8 - (len & 0b0111)) & 0b0111;
  return len + pad_len;
}

bool Encrypt(u8* cipher,
             usize& cipher_len,
             const u8* plaintext,
             usize plaintext_len,
             const u8* key) {
  u32 k[4];
  ParseBigEndianKey(k, key);

  usize len = FIXED_PADDING_LEN + plaintext_len;
  usize pad_len = (8 - (len & 0b0111)) & 0b0111;
  len += pad_len;

  usize header_len = 1 + pad_len + SALT_LEN;

  Vec<u8> encrypted(len);

  // Let's make it faster by using native int types...
  u64 iv2, next_iv2;

  std::random_device rd;
  independent_u8_engine generator(rd());
  std::generate_n(encrypted.data(), header_len, [&generator]() {
    return static_cast<std::uint8_t>(generator());
  });

  encrypted[0] = (encrypted[0] & 0b1111'1000) | (pad_len & 0b0000'0111);
  std::copy_n(plaintext, plaintext_len, &encrypted[header_len]);

  // Process first block
  iv2 = *reinterpret_cast<u64*>(&encrypted[0]);
  ecb::EncryptBlock(&encrypted[0], k);

  for (usize i = 8; i < len; i += 8) {
    // XOR previous encrypted block
    XorInt<u64>(&encrypted[i], &encrypted[i - 8]);

    // store iv2
    next_iv2 = *reinterpret_cast<const u64*>(&encrypted[i]);

    // TEA ECB
    ecb::EncryptBlock(&encrypted[i], k);

    // XOR iv2
    *reinterpret_cast<u64*>(&encrypted[i]) ^= iv2;

    iv2 = next_iv2;
  }

  std::copy(encrypted.begin(), encrypted.end(), cipher);
  cipher_len = encrypted.size();

  return true;
}

}  // namespace umc::misc::tc_tea::cbc
