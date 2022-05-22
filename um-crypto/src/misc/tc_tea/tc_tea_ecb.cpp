#include "tc_tea.h"

#include "um-crypto/endian.h"

#include <algorithm>

namespace umc::misc::tc_tea::ecb {

constexpr u32 ROUNDS = 16;
constexpr u32 DELTA = 0x9e3779b9;

inline u32 single_round_arithmetic(u32 value, u32 sum, u32 key1, u32 key2) {
  return ((value << 4) + key1) ^ (value + sum) ^ ((value >> 5) + key2);
}

void DecryptBlock(void* block, u32* k) {
  auto block_u32 = reinterpret_cast<u32*>(block);

  u32 y = SwapBigEndianToHost(block_u32[0]);
  u32 z = SwapBigEndianToHost(block_u32[1]);
  u32 sum = DELTA * ROUNDS;

  for (int i = 0; i < ROUNDS; i++) {
    z -= single_round_arithmetic(y, sum, k[2], k[3]);
    y -= single_round_arithmetic(z, sum, k[0], k[1]);
    sum -= DELTA;
  }

  block_u32[0] = SwapHostToBigEndian(y);
  block_u32[1] = SwapHostToBigEndian(z);
}

}  // namespace umc::misc::tc_tea::ecb
