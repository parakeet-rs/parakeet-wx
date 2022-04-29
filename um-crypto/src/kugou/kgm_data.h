#pragma once
#include <cstdint>

// Internal header file

namespace kgm_data {

const size_t table_size = 16 * 17;
const size_t vpr_key_size = 17;

extern uint8_t t1[table_size];
extern uint8_t t2[table_size];
extern uint8_t v2[table_size];
extern uint8_t vpr_key[vpr_key_size];

};  // namespace kgm_data
