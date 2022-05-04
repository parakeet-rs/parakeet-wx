#include "um-crypto/tencent/AEKeyDerive.h"

#include "../internal/StringHelper.h"
#include "tc_tea/tc_tea_cbc.h"
#include "um-crypto/qmcv2.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <boost/beast/core/detail/base64.hpp>

namespace base64 = boost::beast::detail::base64;

namespace umc::tencent {

inline Vec<u8> AEKeyDerive::DeriveTEAKey(const Vec<u8> ekey) const {
  Vec<u8> tea_key(16);
  Vec<u8> simple_key(8);
  MakeSimpleKey(simple_key);

  for (int i = 0; i < 16; i += 2) {
    tea_key[i + 0] = simple_key[i / 2];
    tea_key[i + 1] = ekey[i / 2];
  }

  return tea_key;
}

bool AEKeyDerive::FromEKey(Vec<u8>& out, const Str ekey_b64) const {
  std::string ekey_str(ekey_b64);
  RemoveWhitespace(ekey_str);

  std::vector<uint8_t> ekey(base64::decoded_size(ekey_str.size()));
  auto result = base64::decode(ekey.data(), ekey_str.data(), ekey_str.size());
  ekey.resize(result.first);

  return FromEKey(out, ekey);
}

bool AEKeyDerive::FromEKey(Vec<u8>& out, const Vec<u8> ekey) const {
  const auto ekey_len = ekey.size();

  if (ekey_len < 8) {
    out.resize(0);
    return false;
  }

  auto tea_key = DeriveTEAKey(ekey);
  out.resize(ekey_len);
  memcpy(out.data(), ekey.data(), 8u);

  auto data_len = ekey_len - 8;
  auto p_key = tea_key.data();

  size_t out_len;
  if (!tc_tea::cbc_decrypt(&out[8], out_len, &ekey[8], data_len, p_key)) {
    out.resize(0);
    return false;
  };

  out.resize(8 + out_len);
  return true;
}

}  // namespace umc::tencent
