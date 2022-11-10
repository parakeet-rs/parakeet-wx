#pragma once

#include "um-crypto/types.h"

#include <span>

namespace umc::utils {

std::vector<u8> Base64Decode(const std::string& input);
std::string Base64Encode(const std::vector<u8>& input);
std::vector<u8> Base64EncodeBytes(const std::span<const u8> data);

}  // namespace umc::utils
