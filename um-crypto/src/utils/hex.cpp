#include "um-crypto/utils/hex.h"
#include "um-crypto/utils/StringHelper.h"

#include <cryptopp/hex.h>

namespace umc::utils {

std::string Hex(const std::vector<uint8_t>& v) {
  CryptoPP::HexEncoder encoder(nullptr, false, 2, " ");
  encoder.Put(v.data(), v.size());
  encoder.MessageEnd();

  std::string result(encoder.MaxRetrievable(), 0);
  encoder.Get(reinterpret_cast<uint8_t*>(result.data()), result.size());
  return result;
}

std::string HexCompactLowercase(const std::span<const uint8_t> data) {
  CryptoPP::HexEncoder encoder(nullptr, false, 0, "", "");
  encoder.Put(data.data(), data.size());
  encoder.MessageEnd();

  std::string result(encoder.MaxRetrievable(), 0);
  encoder.Get(reinterpret_cast<uint8_t*>(result.data()), result.size());
  return result;
}

std::vector<uint8_t> Unhex(const std::string& str) {
  CryptoPP::HexDecoder decoder;
  decoder.Put(reinterpret_cast<const uint8_t*>(str.data()), str.size());
  decoder.MessageEnd();

  std::vector<uint8_t> result(decoder.MaxRetrievable());
  decoder.Get(result.data(), result.size());
  return result;
}

}  // namespace umc::utils
