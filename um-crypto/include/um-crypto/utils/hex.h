#pragma once

#include "um-crypto/types.h"

#include <span>

namespace umc::utils {

Str Hex(const Vec<u8>& v);
Str HexCompactLowercase(const std::span<const u8> data);
Vec<u8> Unhex(const Str& v);

}  // namespace umc::utils
