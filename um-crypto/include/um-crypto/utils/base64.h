#pragma once

#include "um-crypto/types.h"

#include <span>

namespace umc::utils {

Vec<u8> Base64Decode(const Str& input);
Str Base64Encode(const Vec<u8>& input);
Vec<u8> Base64EncodeBytes(const std::span<const u8> data);

}  // namespace umc::utils
