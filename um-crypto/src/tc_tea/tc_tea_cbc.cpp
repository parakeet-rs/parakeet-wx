#include "tc_tea_ecb.h"
#include "um-crypto/endian.h"

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace umc::tc_tea {

const size_t SALT_LEN = 2;
const size_t ZERO_LEN = 7;
const size_t FIXED_PADDING_LEN = 1 + SALT_LEN + ZERO_LEN;

void cbc_parse_key(uint32_t* result, const uint8_t* key) {
  auto key_u32_be = reinterpret_cast<const uint32_t*>(key);

  for (int i = 0; i < 4; i++) {
    result[i] = umc_betoh_u32(key_u32_be[i]);
  }
}

bool cbc_decrypt(uint8_t* plaindata,
                 size_t& plaindata_len,
                 const uint8_t* cipher,
                 size_t cipher_len,
                 const uint8_t* key) {
  plaindata_len = 0;
  uint32_t k[4];
  cbc_parse_key(k, key);

  if (cipher_len < FIXED_PADDING_LEN || cipher_len % 8 != 0) {
    return false;
  }

  uint8_t* decrypted = static_cast<uint8_t*>(malloc(cipher_len));
  memcpy(decrypted, cipher, cipher_len);

  // decrypt first block
  ecb_decrypt(decrypted, k);
  for (size_t i = 8; i < cipher_len; i += 8) {
    // xor with previous block first
    for (int j = i - 8; j < i; j++) {
      decrypted[j + 8] ^= decrypted[j];
    }

    ecb_decrypt(&decrypted[i], k);
  }

  for (size_t i = 8; i < cipher_len; i++) {
    decrypted[i] ^= cipher[i - 8];
  }

  size_t pad_size = size_t(decrypted[0] & 0b111);
  size_t start_loc = 1 + pad_size + SALT_LEN;
  size_t end_loc = cipher_len - ZERO_LEN;

  plaindata_len = end_loc - start_loc;
  memcpy(plaindata, &decrypted[start_loc], plaindata_len);

  free(decrypted);

  return true;
}

}  // namespace umc::tc_tea
