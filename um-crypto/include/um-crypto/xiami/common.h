#pragma once

#include "um-crypto/types.h"

namespace umc::xiami {

constexpr usize kXiamiHeaderSize = 0x10;
typedef Arr<u8, kXiamiHeaderSize> XiamiHeader;

constexpr usize kXiamiSkipBOFSize = 0x10;

struct XiamiXMConfig {
  u8 file_key;
  usize noop_len;
};

static_assert(usize(1 << 23) == u32(1 << 23),
              "usize should be able to hold 24 bit integer.");

}  // namespace umc::xiami
