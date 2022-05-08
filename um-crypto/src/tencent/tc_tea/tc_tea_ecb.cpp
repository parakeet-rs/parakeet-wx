#include "tc_tea_ecb.h"
#include "um-crypto/endian.h"

namespace umc::tc_tea {

constexpr uint32_t ROUNDS = 16;
constexpr uint32_t DELTA = 0x9e3779b9;

inline uint32_t single_round_arithmetic(uint32_t value,
                                        uint32_t sum,
                                        uint32_t key1,
                                        uint32_t key2) {
  return ((value << 4) + key1) ^ (value + sum) ^ ((value >> 5) + key2);
}

void ecb_decrypt(void* block, uint32_t* k) {
  auto block_u32 = reinterpret_cast<uint32_t*>(block);

  uint32_t y = SwapBigEndianToHost(block_u32[0]);
  uint32_t z = SwapBigEndianToHost(block_u32[1]);
  uint32_t sum = DELTA * ROUNDS;

  for (int i = 0; i < ROUNDS; i++) {
    z -= single_round_arithmetic(y, sum, k[2], k[3]);
    y -= single_round_arithmetic(z, sum, k[0], k[1]);
    sum -= DELTA;
  }

  block_u32[0] = SwapHostToBigEndian(y);
  block_u32[1] = SwapHostToBigEndian(z);
}

}  // namespace umc::tc_tea
