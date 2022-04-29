#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

inline uint8_t rotate(uint8_t value, int bits) {
  int shift_amount = (bits + 4) % 8;
  auto left = value << shift_amount;
  auto right = value >> shift_amount;
  return uint8_t(left | right);
}

typedef struct qmcv2_map {
  uint8_t* key;
  size_t N;
} qmcv2_map;

qmcv2_map* qmcv2_map_init(const uint8_t* key, const size_t key_size) {
  auto ctx = static_cast<qmcv2_map*>(calloc(1, sizeof(qmcv2_map)));

  ctx->key = static_cast<uint8_t*>(calloc(key_size, sizeof(uint8_t)));
  memcpy(ctx->key, key, key_size);

  ctx->N = key_size;

  return ctx;
}

uint8_t qmcv2_map_encrypt(qmcv2_map* ctx, size_t offset) {
  if (offset > 0x7FFF)
    offset %= 0x7FFF;

  uint64_t idx = (offset * offset + 71214) % ctx->N;
  uint8_t value = ctx->key[idx];

  // Rotate by the lower 3 bits
  return rotate(value, idx & 0b0111);
}

uint8_t qmcv2_map_decrypt(qmcv2_map* ctx, size_t offset) {
  return qmcv2_map_encrypt(ctx, offset);
}

void qmcv2_map_free(qmcv2_map*& ctx) {
  assert(ctx && "qmcv2_map_free: ctx is nullptr");

  free(ctx->key);
  memset(ctx, 0xff, sizeof(qmcv2_map));
  free(ctx);

  ctx = nullptr;
}
