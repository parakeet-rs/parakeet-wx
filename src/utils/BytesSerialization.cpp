#include "BytesSerialization.h"

#include <parakeet-crypto/utils/base64.h>
#include <parakeet-crypto/utils/hex.h>

#include <cctype>

#include <algorithm>
#include <string>

namespace parakeet_wx::utils {

using parakeet_crypto::utils::Base64Decode;
using parakeet_crypto::utils::Base64Encode;

using parakeet_crypto::utils::Hex;
using parakeet_crypto::utils::UnHex;

std::string SerializeBytes(std::span<const uint8_t> data) {
  if (data.size() == 0) {
    return "";
  }

  if (std::ranges::all_of(data, isprint)) {
    return std::string("raw:") + std::string(data.begin(), data.end());
  }

  if (data.size() <= 16) {
    return std::string("hex:") + Hex(data);
  }

  return std::string(Base64Encode(data));
}
std::vector<uint8_t> DeserializeBytes(const std::string str) {
  if (str.empty()) return {};

  if (str.starts_with("raw:")) {
    return std::vector<uint8_t>(str.begin() + 4, str.end());
  }

  if (str.starts_with("hex:")) {
    return UnHex(std::span{str.begin() + 4, str.size() - 4});
  }

  return Base64Decode(str);
}

}  // namespace parakeet_wx::utils
