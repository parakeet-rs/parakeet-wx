#pragma once

#include "um-crypto/common.h"

#include <array>

namespace umc::kugou {

const usize kTableSize = 16 * 17;

// vpr had its own key on top of the file key.
const usize kFileKeySize = 17;
const usize kVprKeySize = kFileKeySize;
typedef std::array<u8, kFileKeySize> KugouFileKey;

// table t1, t2, v2
typedef std::array<u8, kTableSize> KugouTable;

// file header - either kgm or vpr.
const usize kFormatHeaderSize = 16;
typedef std::array<u8, kFormatHeaderSize> KugouHeader;
extern const KugouHeader kFileHeaderKGM;
extern const KugouHeader kFileHeaderVPR;

}  // namespace umc::kugou
