#include "um-crypto/types.h"
#include "um-crypto/utils/StringHelper.h"

#include <boost/algorithm/hex.hpp>

namespace umc::utils {

Str Hex(const Vec<u8>& v) {
  const char* kHexChar = "0123456789abcdef";

  usize len = v.size();
  Str result;
  result.resize(len * 3, ' ');

  const u8* p_in = v.data();
  char* p_out = result.data();

  for (usize i = 0; i < len; i++, p_out++) {
    u8 v = *p_in++;
    *p_out++ = kHexChar[v >> 4];
    *p_out++ = kHexChar[v & 0x0f];

    if ((i & 0xf) == 0x0f) {
      *p_out = '\n';
    }
  }

  return result;
}

Vec<u8> Unhex(const Str& str) {
  Vec<u8> result;
  boost::algorithm::unhex(RemoveWhitespace(str), std::back_inserter(result));
  return result;
}

}  // namespace umc::utils
