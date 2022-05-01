#pragma once
#include "um-crypto/common.h"

// Internal header file

namespace umc::kugou {

const usize table_size = 16 * 17;
const usize vpr_key_size = 17;
const usize file_key_size = 17;

extern u8 t1[table_size];
extern u8 t2[table_size];
extern u8 v2[table_size];
extern u8 vpr_key[vpr_key_size];

};  // namespace umc::kugou
