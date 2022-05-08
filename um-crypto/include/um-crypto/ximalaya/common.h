#pragma once
#include "um-crypto/types.h"

namespace umc::ximalaya {

constexpr usize kXimalayaHeaderContentKeySize = 0x20;
constexpr usize kXimalayaHeaderContentKeyIndexMask = 0x1f;  // 0x20 - 1
constexpr usize kXimalayaEncryptedHeaderSize = 0x400;

static_assert(kXimalayaHeaderContentKeySize % 4 == 0,
              "kXimalayaHeaderContentKeySize should be multiple of 4");

typedef Arr<u8, 4> XimalayaX2MContentKey;
typedef Arr<u8, kXimalayaHeaderContentKeySize> XimalayaHeaderContentKey;

typedef Arr<u8, kXimalayaEncryptedHeaderSize> XimalayaAndroidFileHeader;
typedef Arr<u16, kXimalayaEncryptedHeaderSize> XimalayaHeaderScrambleTable;

}  // namespace umc::ximalaya
