#pragma once

#include "um-crypto/types.h"

namespace umc::utils {

Vec<u8> Base64Decode(const Str& input);
Str Base64Encode(const Vec<u8>& input);

}  // namespace umc::utils
