#pragma once

#include "um-crypto/types.h"

#include <span>

namespace umc::utils {

std::string Hex(const std::vector<u8>& v);
std::string HexCompactLowercase(const std::span<const u8> data);
std::vector<u8> Unhex(const std::string& v);

}  // namespace umc::utils
