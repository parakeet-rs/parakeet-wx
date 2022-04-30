#include "um-crypto/qmcv2.h"
#include "um-crypto/tc_tea.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>

#include <string>
#include <vector>

#include <boost/beast/core/detail/base64.hpp>

namespace base64 = boost::beast::detail::base64;

namespace umc::qmc::v2::key_derive {

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

bool from_ekey(uint8_t*& derived_key,
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

  size_t tea_decrypted_len;
  if (!umc::tc_tea::cbc_decrypt(&derived_key[8], tea_decrypted_len, &ekey[8],
                                ekey_len - 8, tea_key)) {
    free(derived_key);
    derived_key = nullptr;
    return false;
  };

  derived_key_len = 8 + tea_decrypted_len;

  return true;
}

bool from_ekey_b64(uint8_t*& derived_key,
                   size_t& derived_key_len,
                   const char* ekey_b64) {
  std::string ekey_str(ekey_b64);

  // Remove whitespace
  ekey_str.erase(std::remove_if(ekey_str.begin(), ekey_str.end(), ::isspace),
                 ekey_str.end());

  std::vector<uint8_t> ekey(base64::decoded_size(ekey_str.size()));
  auto result = base64::decode(ekey.data(), ekey_str.data(), ekey_str.size());
  return from_ekey(derived_key, derived_key_len, ekey.data(), result.first);
}

void free_key(uint8_t*& key) {
  assert(key && "umc_qmcv2_free_derived_key: key is nullptr");

  free(key);
};

}  // namespace umc::qmc::v2::key_derive
