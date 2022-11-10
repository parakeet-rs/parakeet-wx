#include "um-crypto/utils/hex.h"
#include "um-crypto/types.h"
#include "um-crypto/utils/StringHelper.h"

#include <cryptopp/hex.h>

namespace umc::utils {

std::string Hex(const std::vector<u8>& v) {
  CryptoPP::HexEncoder encoder(nullptr, false, 2, " ");
  encoder.Put(v.data(), v.size());
  encoder.MessageEnd();

  std::string result(encoder.MaxRetrievable(), 0);
  encoder.Get(reinterpret_cast<u8*>(result.data()), result.size());
  return result;
}

std::string HexCompactLowercase(const std::span<const u8> data) {
  CryptoPP::HexEncoder encoder(nullptr, false, 0, "", "");
  encoder.Put(data.data(), data.size());
  encoder.MessageEnd();

  std::string result(encoder.MaxRetrievable(), 0);
  encoder.Get(reinterpret_cast<u8*>(result.data()), result.size());
  return result;
}

std::vector<u8> Unhex(const std::string& str) {
  CryptoPP::HexDecoder decoder;
  decoder.Put(reinterpret_cast<const u8*>(str.data()), str.size());
  decoder.MessageEnd();

  std::vector<u8> result(decoder.MaxRetrievable());
  decoder.Get(result.data(), result.size());
  return result;
}

}  // namespace umc::utils
