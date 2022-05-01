#include "um-crypto/qmcv2/rc4_cipher.h"
#include "../internal/inline.h"

#include <algorithm>

using namespace umc;
using namespace umc::qmcv2;

const usize FIRST_SEGMENT_SIZE = 0x80;
const usize OTHER_SEGMENT_SIZE = 0x1400;

RC4Cipher::RC4Cipher(const Vec<u8>& key) : IStreamCipher() {
  this->key = key;
  this->N = key.size();
  InitSeedbox();
  this->key_hash = this->CalculateKeyHash();
}

inline double RC4Cipher::CalculateKeyHash() const {
  u32 hash = 1;
  for (u32 i = 0; i < N; i++) {
    auto value = i32{key[i]};

    // ignore if key char is '\x00'
    if (!value)
      continue;

    const u32 next_hash = hash * value;
    if (next_hash == 0 || next_hash <= hash)
      break;

    hash = next_hash;
  }

  return static_cast<double>(hash);
}

inline void RC4Cipher::InitSeedbox() {
  this->S.resize(N);

  for (u32 i = 0; i < N; i++) {
    S[i] = i & 0xFF;
  }

  u32 j = 0;
  for (u32 i = 0; i < N; i++) {
    j = (S[i] + j + key[i % N]) % N;
    std::swap(S[i], S[j]);
  }
}

inline u64 RC4Cipher::GetSegmentKey(u64 segment_id, u64 seed) const {
  return u64(key_hash / double((segment_id + 1) * seed) * 100.0);
}

__umc_fi void RC4Cipher::EncodeFirstSegment(u8* out, const u8* buf, usize len) {
  for (usize i = 0; i < len; i++, offset++) {
    const u64 seed = u64{key[offset % N]};
    out[i] = buf[i] ^ key[GetSegmentKey(offset, seed) % N];
  }
}

__umc_fi void RC4Cipher::EncodeOtherSegment(u8* out, const u8* buf, usize len) {
  const auto N = this->N;

  Vec<u8> S(this->S);

  size_t sid = offset / OTHER_SEGMENT_SIZE;
  size_t segment_seed = key[sid & 0x1FF];

  // segment_key contains the number of bytes to discard during rc4 init.
  auto segment_offset = offset % OTHER_SEGMENT_SIZE;
  auto discards = segment_offset + (GetSegmentKey(sid, segment_seed) & 0x1FF);

  u32 j = 0;
  u32 k = 0;
  for (u32 i = 0; i < discards; i++) {
    j = (j + 1) % N;
    k = (S[j] + k) % N;
    std::swap(S[j], S[k]);
  }

  // Now we manipulate the buffer.
  for (u32 i = 0; i < len; i++) {
    j = (j + 1) % N;
    k = (S[j] + k) % N;
    std::swap(S[j], S[k]);

    out[i] = buf[i] ^ S[(S[j] + S[k]) % N];
  }

  offset += len;
}

bool RC4Cipher::Encrypt(Vec<u8>& result, const Vec<u8>& input) {
  result.resize(input.size());

  u8* out_ptr = result.data();
  const u8* inp_ptr = input.data();
  usize len = input.size();

  if (offset < FIRST_SEGMENT_SIZE) {
    const auto bytes_processed = std::min(FIRST_SEGMENT_SIZE - offset, len);
    EncodeFirstSegment(out_ptr, inp_ptr, bytes_processed);
    len -= bytes_processed;
    inp_ptr += bytes_processed;
    out_ptr += bytes_processed;
  }

  if (offset % OTHER_SEGMENT_SIZE != 0) {
    const auto segment_offset = offset % OTHER_SEGMENT_SIZE;
    const auto bytes_processed =
        std::min(OTHER_SEGMENT_SIZE - segment_offset, len);
    EncodeOtherSegment(out_ptr, inp_ptr, bytes_processed);
    len -= bytes_processed;
    inp_ptr += bytes_processed;
    out_ptr += bytes_processed;
  }

  while (len > 0) {
    auto bytes_processed = std::min(OTHER_SEGMENT_SIZE, len);
    EncodeOtherSegment(out_ptr, inp_ptr, bytes_processed);
    len -= bytes_processed;
    inp_ptr += bytes_processed;
    out_ptr += bytes_processed;
  }

  return true;
}

inline bool RC4Cipher::Decrypt(Vec<u8>& result, const Vec<u8>& input) {
  return Encrypt(result, input);
}
