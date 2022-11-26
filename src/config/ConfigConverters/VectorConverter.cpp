#include "VectorConverter.h"

#include <cctype>

#include <algorithm>
#include <string>

#include <parakeet-crypto/utils/base64.h>
#include <parakeet-crypto/utils/hex.h>

NLOHMANN_JSON_NAMESPACE_BEGIN
namespace utils = ::parakeet_crypto::utils;

void adl_serializer<std::vector<uint8_t>>::to_json(json& j, const std::vector<uint8_t>& data) {
  if (data.size() == 0) {
    j = json::array();
  } else if (std::ranges::all_of(data, isprint)) {
    j = std::string("raw:") + std::string(data.begin(), data.end());
  } else if (data.size() <= 16) {
    j = std::string("hex:") + utils::Hex(data);
  } else {
    j = std::string(utils::Base64Encode(data));
  }
}

void adl_serializer<std::vector<uint8_t>>::from_json(const json& j, std::vector<uint8_t>& data) {
  if (j.is_array()) {
    data.resize(j.size());
    std::copy(j.begin(), j.end(), data.begin());
  } else if (j.is_string()) {
    auto str = j.get<std::string>();
    if (str.empty()) return;

    if (str.starts_with("raw:")) {
      data.assign(str.begin() + 4, str.end());
    } else if (str.starts_with("hex:")) {
      str.erase(0, 4);
      data = utils::Unhex(str);
    } else {
      data = utils::Base64Decode(str);
    }
  }
}

NLOHMANN_JSON_NAMESPACE_END
