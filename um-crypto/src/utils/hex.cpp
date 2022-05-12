#include "um-crypto/types.h"
#include "um-crypto/utils/StringHelper.h"

#include <cryptopp/hex.h>

namespace umc::utils {

Str Hex(const Vec<u8>& v) {
  CryptoPP::HexEncoder encoder(nullptr, false, 2, " ");
  encoder.Put(v.data(), v.size());
  encoder.MessageEnd();

  Str result(encoder.MaxRetrievable(), 0);
  encoder.Get(reinterpret_cast<u8*>(result.data()), result.size());
  return result;
}

Vec<u8> Unhex(const Str& str) {
  CryptoPP::HexDecoder decoder;
  decoder.Put(reinterpret_cast<const u8*>(str.data()), str.size());
  decoder.MessageEnd();

  Vec<u8> result(decoder.MaxRetrievable());
  decoder.Get(result.data(), result.size());
  return result;
}

}  // namespace umc::utils
