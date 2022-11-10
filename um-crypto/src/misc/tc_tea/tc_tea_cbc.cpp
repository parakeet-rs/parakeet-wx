#include "tc_tea.h"

#include "internal/XorHelper.h"
#include "um-crypto/endian.h"

#include <algorithm>
#include <random>

namespace umc::misc::tc_tea::cbc {

constexpr std::size_t SALT_LEN = 2;
constexpr std::size_t ZERO_LEN = 7;
constexpr std::size_t FIXED_PADDING_LEN = 1 + SALT_LEN + ZERO_LEN;

void ParseBigEndianKey(uint32_t* result, const uint8_t* key) {
  auto key_u32_be = reinterpret_cast<const uint32_t*>(key);

  for (int i = 0; i < 4; i++) {
    result[i] = SwapBigEndianToHost(key_u32_be[i]);
  }
}

bool Decrypt(uint8_t* plaindata,
             std::size_t& plaindata_len,
             const uint8_t* cipher,
             std::size_t cipher_len,
             const uint8_t* key) {
  plaindata_len = 0;
  uint32_t k[4];
  ParseBigEndianKey(k, key);

  if (cipher_len < FIXED_PADDING_LEN || cipher_len % 8 != 0) {
    return false;
  }

  std::vector<uint8_t> decrypted_v(cipher_len);
  uint8_t* decrypted = decrypted_v.data();
  std::copy_n(cipher, cipher_len, decrypted);

  // decrypt first block
  ecb::DecryptBlock(decrypted, k);
  for (std::size_t i = 8; i < cipher_len; i += 8) {
    // xor with previous block first
    XorInt<uint64_t>(&decrypted[i], &decrypted[i - 8]);
    ecb::DecryptBlock(&decrypted[i], k);
  }

  // Hint compiler that we are XOR block of 8.
  for (std::size_t i = 8; i < cipher_len; i += 8) {
    XorInt<uint64_t>(&decrypted[i], &cipher[i - 8]);
  }

  std::size_t pad_size = std::size_t(decrypted[0] & 0b111);
  std::size_t start_loc = 1 + pad_size + SALT_LEN;
  std::size_t end_loc = cipher_len - ZERO_LEN;

  plaindata_len = end_loc - start_loc;
  std::copy_n(&decrypted[start_loc], plaindata_len, plaindata);

  // Constant time zero check
  uint8_t zero_sum = 0;
  for (std::size_t i = 0; i < ZERO_LEN; i++) {
    zero_sum |= decrypted[end_loc + i];
  }

  return zero_sum == 0;
}

std::size_t GetEncryptedSize(std::size_t size) {
  std::size_t len = FIXED_PADDING_LEN + size;
  std::size_t pad_len = (8 - (len & 0b0111)) & 0b0111;
  return len + pad_len;
}

bool Encrypt(uint8_t* cipher,
             std::size_t& cipher_len,
             const uint8_t* plaintext,
             std::size_t plaintext_len,
             const uint8_t* key) {
  uint32_t k[4];
  ParseBigEndianKey(k, key);

  std::size_t len = FIXED_PADDING_LEN + plaintext_len;
  std::size_t pad_len = (8 - (len & 0b0111)) & 0b0111;
  len += pad_len;

  std::size_t header_len = 1 + pad_len + SALT_LEN;

  std::vector<uint8_t> encrypted(len);

  // Let's make it faster by using native int types...
  uint64_t iv2, next_iv2;

  using random_bit_engine =
      std::independent_bits_engine<std::default_random_engine, std::numeric_limits<uint8_t>::digits, uint16_t>;

  std::random_device rd;
  random_bit_engine rbe(rd());
  std::generate_n(encrypted.data(), header_len, [&rbe]() { return static_cast<std::uint8_t>(rbe()); });

  encrypted[0] = (encrypted[0] & 0b1111'1000) | (pad_len & 0b0000'0111);
  std::copy_n(plaintext, plaintext_len, &encrypted[header_len]);

  // Process first block
  iv2 = *reinterpret_cast<uint64_t*>(&encrypted[0]);
  ecb::EncryptBlock(&encrypted[0], k);

  for (std::size_t i = 8; i < len; i += 8) {
    // XOR previous encrypted block
    XorInt<uint64_t>(&encrypted[i], &encrypted[i - 8]);

    // store iv2
    next_iv2 = *reinterpret_cast<const uint64_t*>(&encrypted[i]);

    // TEA ECB
    ecb::EncryptBlock(&encrypted[i], k);

    // XOR iv2
    *reinterpret_cast<uint64_t*>(&encrypted[i]) ^= iv2;

    iv2 = next_iv2;
  }

  std::copy(encrypted.begin(), encrypted.end(), cipher);
  cipher_len = encrypted.size();

  return true;
}

}  // namespace umc::misc::tc_tea::cbc
