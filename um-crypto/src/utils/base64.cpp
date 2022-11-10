#include "um-crypto/utils/base64.h"
#include "um-crypto/utils/StringHelper.h"

#include <cryptopp/base64.h>

namespace umc::utils {

std::vector<u8> Base64Decode(const std::string& input) {
  CryptoPP::Base64Decoder decoder;
  decoder.Put(reinterpret_cast<const u8*>(input.data()), input.size());
  decoder.MessageEnd();

  std::vector<u8> result(decoder.MaxRetrievable());
  decoder.Get(result.data(), result.size());
  return result;
}

std::string Base64Encode(const std::vector<u8>& input) {
  CryptoPP::Base64Encoder encoder(nullptr, false);
  encoder.Put(input.data(), input.size());
  encoder.MessageEnd();

  std::string result(encoder.MaxRetrievable(), 0);
  encoder.Get(reinterpret_cast<u8*>(result.data()), result.size());
  return result;
}

std::vector<u8> Base64EncodeBytes(const std::span<const u8> data) {
  CryptoPP::Base64Encoder encoder(nullptr, false);
  encoder.Put(data.data(), data.size());
  encoder.MessageEnd();

  std::vector<u8> result(encoder.MaxRetrievable(), 0);
  encoder.Get(result.data(), result.size());
  return result;
}

}  // namespace umc::utils
