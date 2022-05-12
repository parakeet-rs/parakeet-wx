#include "um-crypto/utils/base64.h"
#include "um-crypto/utils/StringHelper.h"

#include <cryptopp/base64.h>

namespace umc::utils {

Vec<u8> Base64Decode(const Str& input) {
  CryptoPP::Base64Decoder decoder;
  decoder.Put(reinterpret_cast<const u8*>(input.data()), input.size());
  decoder.MessageEnd();

  Vec<u8> result(decoder.MaxRetrievable());
  decoder.Get(result.data(), result.size());
  return result;
}

Str Base64Encode(const Vec<u8>& input) {
  CryptoPP::Base64Encoder encoder(nullptr, false);
  encoder.Put(input.data(), input.size());
  encoder.MessageEnd();

  Str result(encoder.MaxRetrievable(), 0);
  encoder.Get(reinterpret_cast<u8*>(result.data()), result.size());
  return result;
}

}  // namespace umc::utils
