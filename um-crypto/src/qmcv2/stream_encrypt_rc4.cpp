#include "um-crypto/qmcv2.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>

const size_t FIRST_SEGMENT_SIZE = 0x80;
const size_t SEGMENT_SIZE = 0x1400;

using umc::qmc::v2::rc4::CTX;

struct CTX {
  // RC4 key
  uint8_t* key;
  size_t N;

  // Seedbox
  uint8_t* S;
  uint32_t key_hash;
};

// Private methods

namespace umc::qmc::v2::rc4 {

// Internal helper methods

void init_seedbox(uint8_t* S, const uint8_t* key, const size_t N) {
  for (size_t i = 0; i < N; ++i) {
    S[i] = i & 0xFF;
  }

  int j = 0;
  for (size_t i = 0; i < N; ++i) {
    j = (S[i] + j + key[i % N]) % N;
    std::swap(S[i], S[j]);
  }
}

uint32_t hash(uint8_t* key, size_t N) {
  uint32_t hash = 1;
  for (size_t i = 0; i < N; i++) {
    auto value = int32_t{key[i]};

    // ignore if key char is '\x00'
    if (!value)
      continue;

    uint32_t next_hash = hash * value;
    if (next_hash == 0 || next_hash <= hash)
      break;

    hash = next_hash;
  }

  return hash;
}

uint64_t get_segment_key(CTX* ctx, size_t sid, uint64_t seed) {
  return uint64_t((double)ctx->key_hash / double((sid + 1) * seed) * 100.0);
}

void encrypt_first_segment(CTX* ctx,
                           uint8_t* buf,
                           size_t buf_len,
                           size_t offset) {
  for (size_t i = 0; i < buf_len; i++, offset++) {
    uint64_t seed = uint64_t{ctx->key[offset % ctx->N]};
    buf[i] ^= ctx->key[get_segment_key(ctx, offset, seed) % ctx->N];
  }
}

void encrypt_other_segment(CTX* ctx,
                           uint8_t* S,
                           uint8_t* buf,
                           size_t buf_len,
                           size_t offset) {
  const auto N = ctx->N;

  // Duplicate a new seedbox
  memcpy(S, ctx->S, N);

  size_t sid = offset / SEGMENT_SIZE;
  size_t segment_seed = ctx->key[sid & 0x1FF];

  // segment_key contains the number of bytes to discard during rc4 init.
  auto discard_len = get_segment_key(ctx, sid, segment_seed) & 0x1FF;

  // additionally, we discarda more bytes after segment alignment.
  discard_len += offset % SEGMENT_SIZE;

  int j = 0;
  int k = 0;
  for (size_t i = 0; i < discard_len; i++) {
    j = (j + 1) % N;
    k = (S[j] + k) % N;
    std::swap(S[j], S[k]);
  }

  // Now we manipulate the buffer.
  for (size_t i = 0; i < buf_len; i++) {
    j = (j + 1) % N;
    k = (S[j] + k) % N;
    std::swap(S[j], S[k]);

    buf[i] ^= S[(S[j] + S[k]) % N];
  }
}

// Implementation

CTX* umc::qmc::v2::rc4::new_from_key(const uint8_t* key, size_t key_size) {
  CTX* ctx = static_cast<CTX*>(calloc(1, sizeof(CTX)));

  ctx->S = static_cast<uint8_t*>(calloc(key_size, sizeof(uint8_t)));
  ctx->N = key_size;
  ctx->key = static_cast<uint8_t*>(calloc(key_size, sizeof(uint8_t)));
  memcpy(ctx->key, key, key_size);

  umc::qmc::v2::rc4::init_seedbox(ctx->S, ctx->key, key_size);
  ctx->key_hash = umc::qmc::v2::rc4::hash(ctx->key, key_size);

  return ctx;
}

void umc::qmc::v2::rc4::encrypt(CTX* ctx,
                                uint8_t* buf,
                                size_t len,
                                size_t offset) {
  uint8_t* buf_end = buf + len;

  if (offset < FIRST_SEGMENT_SIZE) {
    auto len_segment = std::min(len, FIRST_SEGMENT_SIZE - offset);
    umc::qmc::v2::rc4::encrypt_first_segment(ctx, buf, len_segment, offset);
    len -= len_segment;
    buf += len_segment;
    offset += len_segment;
  }

  auto s_temp = static_cast<uint8_t*>(calloc(ctx->N, sizeof(uint8_t)));
  if (offset % SEGMENT_SIZE != 0) {
    auto len_segment = std::min(SEGMENT_SIZE - (offset % SEGMENT_SIZE), len);
    umc::qmc::v2::rc4::encrypt_other_segment(ctx, s_temp, buf, len_segment,
                                             offset);
    len -= len_segment;
    buf += len_segment;
    offset += len_segment;
  }

  // Batch process segments
  while (len > SEGMENT_SIZE) {
    auto len_segment = std::min(size_t{SEGMENT_SIZE}, len);
    umc::qmc::v2::rc4::encrypt_other_segment(ctx, s_temp, buf, len_segment,
                                             offset);
    len -= len_segment;
    buf += len_segment;
    offset += len_segment;
  }

  if (len > 0) {
    umc::qmc::v2::rc4::encrypt_other_segment(ctx, s_temp, buf, len, offset);
    buf += len;
  }

  ::free(s_temp);
  assert(buf == buf_end);
}

void umc::qmc::v2::rc4::decrypt(CTX* ctx,
                                uint8_t* buf,
                                size_t len,
                                size_t offset) {
  return umc::qmc::v2::rc4::encrypt(ctx, buf, len, offset);
}

void umc::qmc::v2::rc4::free(CTX*& ctx) {
  assert(ctx && "qmcv2_rc4_free: ctx is nullptr");

  ::free(ctx->key);
  ::free(ctx->S);
  memset(ctx, 0xff, sizeof(CTX));
  ::free(ctx);

  ctx = nullptr;
}

}  // namespace umc::qmc::v2::rc4
