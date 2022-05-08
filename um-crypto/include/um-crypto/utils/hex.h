#pragma once

#include "um-crypto/types.h"

namespace umc::utils {

Str Hex(const Vec<u8>& v);
Vec<u8> Unhex(const Str& v);

}  // namespace umc::utils
