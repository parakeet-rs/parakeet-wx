#include <openssl/evp.h>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

void make_simple_key(uint8_t seed, size_t len, uint8_t* buf) {
  for (int i = 0; len > i; ++i) {
    buf[i] = (uint8_t)(fabs(tan((float)seed + (double)i * 0.1)) * 100.0);
  }
}

void derive_tea_key(uint8_t* tea_key, uint8_t* ekey) {
  uint8_t simple_key[8];
  make_simple_key(106, 8, simple_key);

  for (int i = 0; i < 16; i += 2) {
    tea_key[i + 0] = simple_key[i / 2];
    tea_key[i + 1] = ekey[i / 2];
  }
}

bool umc_qmcv2_derive_from_ekey(uint8_t*& key, uint8_t* ekey, size_t ekey_len) {
  if (ekey_len < 8) {
    return false;
  }

  uint8_t tea_key[16];
  derive_tea_key(tea_key, ekey);

  key = static_cast<uint8_t*>(calloc(ekey_len * 2, sizeof(uint8_t)));
  memcpy(key, ekey, 8u);

  // TODO: implement this

  return true;
}

const unsigned char b64_pad_str[] = "===";
bool umc_qmcv2_derive_from_ekey_b64(uint8_t*& key, char* ekey_b64) {
  size_t ekey_b64_len = strlen(ekey_b64);
  size_t partial_last_block_len = ekey_b64_len % 4;
  size_t ekey_max_len = ekey_b64_len / 4 * 3 + 4;
  uint8_t* ekey = static_cast<uint8_t*>(calloc(ekey_max_len, sizeof(uint8_t)));

  int ekey_len = 0;
  auto b64 = EVP_ENCODE_CTX_new();
  EVP_DecodeInit(b64);

  if (-1 == EVP_DecodeUpdate(b64, ekey, &ekey_len,
                             reinterpret_cast<unsigned char*>(ekey_b64),
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

  bool ok = umc_qmcv2_derive_from_ekey(key, ekey, size_t(ekey_len));

  free(ekey);

  return ok;
}
