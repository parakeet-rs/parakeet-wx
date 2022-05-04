#pragma once
#include "um-crypto/common.h"

#include <array>

namespace umc::ximalaya {

constexpr usize kX2MKeySize = 0x400;
constexpr usize kX2MEncryptedHeaderSize = 0x400;

typedef std::array<u8, kX2MEncryptedHeaderSize> X2MHeader;
typedef std::array<u16, kX2MKeySize> X2MScrambleTable;

}  // namespace umc::ximalaya
