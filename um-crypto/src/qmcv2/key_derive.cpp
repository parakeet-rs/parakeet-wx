#include <openssl/evp.h>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include "um-crypto/tc_tea.h"

/**
 * @brief A pseudo-random key derive.
 *
 * @param buf Output buffer.
 * @param seed Seed to use.
 * @param len Number of bytes to generate.
 */
void make_simple_key(uint8_t* buf, uint8_t seed, size_t len) {
  for (int i = 0; len > i; ++i) {
    buf[i] = (uint8_t)(fabs(tan((float)seed + (double)i * 0.1)) * 100.0);
  }
}

/**
 * @brief Derive TEA Key used for QMCv2 encoded files.
 *
 * @param tea_key Output key, 16 bytes.
 * @param ekey embedded ekey.
 */
void derive_tea_key(uint8_t* tea_key, const uint8_t* ekey) {
  uint8_t simple_key[8];
  make_simple_key(simple_key, 106, 8);

  for (int i = 0; i < 16; i += 2) {
    tea_key[i + 0] = simple_key[i / 2];
    tea_key[i + 1] = ekey[i / 2];
  }
}

bool umc_qmcv2_derive_from_ekey(uint8_t*& derived_key,
                                size_t& derived_key_len,
                                const uint8_t* ekey,
                                size_t ekey_len) {
  if (ekey_len < 8) {
    return false;
  }

  uint8_t tea_key[16];
  derive_tea_key(tea_key, ekey);

  derived_key = static_cast<uint8_t*>(calloc(ekey_len, sizeof(uint8_t)));
  memcpy(derived_key, ekey, 8u);

  size_t unused;
  if (!umc_tc_tea_cbc_decrypt(&derived_key[8], unused, &ekey[8], ekey_len - 8,
                              tea_key)) {
    free(derived_key);
    derived_key = nullptr;
    return false;
  };

  return true;
}

const unsigned char b64_pad_str[] = "===";
bool umc_qmcv2_derive_from_ekey_b64(uint8_t*& derived_key,
                                    size_t& derived_key_len,
                                    const char* ekey_b64) {
  size_t ekey_b64_len = strlen(ekey_b64);
  size_t partial_last_block_len = ekey_b64_len % 4;
  size_t ekey_max_len = ekey_b64_len / 4 * 3 + 4;
  uint8_t* ekey = static_cast<uint8_t*>(calloc(ekey_max_len, sizeof(uint8_t)));

  int ekey_len = 0;
  auto b64 = EVP_ENCODE_CTX_new();
  EVP_DecodeInit(b64);

  if (-1 == EVP_DecodeUpdate(b64, ekey, &ekey_len,
                             reinterpret_cast<const uint8_t*>(ekey_b64),
                             ekey_b64_len)) {
    EVP_ENCODE_CTX_free(b64);
    free(ekey);
    return false;
  }

  if (partial_last_block_len > 0) {
    int outl;
    if (-1 == EVP_DecodeUpdate(b64, &ekey[ekey_len], &outl, b64_pad_str,
                               4 - partial_last_block_len)) {
      EVP_ENCODE_CTX_free(b64);
      free(ekey);
      return false;
    }
    ekey_len += outl;
  }

  {
    int outl;
    if (EVP_DecodeFinal(b64, &ekey[ekey_len], &outl) == -1) {
      EVP_ENCODE_CTX_free(b64);
      free(ekey);
      return false;
    }
    ekey_len += outl;
  }

  EVP_ENCODE_CTX_free(b64);

  bool ok = umc_qmcv2_derive_from_ekey(derived_key, derived_key_len, ekey,
                                       size_t(ekey_len));

  free(ekey);

  return ok;
}

void umc_qmcv2_free_derived_key(uint8_t*& key) {
  assert(key && "umc_qmcv2_free_derived_key: key is nullptr");

  free(key);
};
