#include "um-crypto/qmcv2.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace umc::qmc::v2::map {

struct CTX {
  uint8_t* key;
  size_t N;
};

inline uint8_t rotate(uint8_t value, int bits) {
  int shift_amount = (bits + 4) % 8;
  auto left = value << shift_amount;
  auto right = value >> shift_amount;
  return uint8_t(left | right);
}

CTX* new_from_key(const uint8_t* key, size_t key_size) {
  auto ctx = static_cast<CTX*>(calloc(1, sizeof(CTX)));

  ctx->key = static_cast<uint8_t*>(calloc(key_size, sizeof(uint8_t)));
  memcpy(ctx->key, key, key_size);

  ctx->N = key_size;

  return ctx;
}

void free(CTX*& ctx) {
  assert(ctx && "ctx_free: CTX is nullptr");

  ::free(ctx->key);
  memset(ctx, 0xff, sizeof(ctx));
  ::free(ctx);

  ctx = nullptr;
}

uint8_t get_key_at_offset(CTX* ctx, size_t offset) {
  if (offset > 0x7FFF)
    offset %= 0x7FFF;

  uint64_t idx = (offset * offset + 71214) % ctx->N;
  uint8_t value = ctx->key[idx];

  // Rotate by the lower 3 bits
  return rotate(value, idx & 0b0111);
}

void encrypt(CTX* ctx, uint8_t* buf, size_t len, size_t offset) {
  auto end = buf + len;
  do {
    *buf ^= get_key_at_offset(ctx, offset);

    buf++;
    offset++;
  } while (buf != end);
}

void decrypt(CTX* ctx, uint8_t* buf, size_t len, size_t offset) {
  encrypt(ctx, buf, len, offset);
}

}  // namespace umc::qmc::v2::map
