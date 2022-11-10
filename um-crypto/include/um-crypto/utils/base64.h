#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace umc::utils {

std::vector<uint8_t> Base64Decode(const std::string& input);
std::string Base64Encode(const std::vector<uint8_t>& input);
std::vector<uint8_t> Base64EncodeBytes(const std::span<const uint8_t> data);

}  // namespace umc::utils
