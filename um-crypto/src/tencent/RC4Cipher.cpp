#include "um-crypto/tencent/RC4Cipher.h"
#include "../internal/inline.h"

#include <algorithm>

using namespace umc;
using namespace umc::tencent;

constexpr usize FIRST_SEGMENT_SIZE = 0x80;
constexpr usize OTHER_SEGMENT_SIZE = 0x1400;

RC4Cipher::RC4Cipher(const Vec<u8>& key) : AXorStreamCipher() {
  key_ = key;
  N_ = key.size();
  InitSeedbox();
  key_hash_ = CalculateKeyHash();

  // pre-alloc buffer.
  buf_.resize(OTHER_SEGMENT_SIZE);

  // Init
  HardSeek(0);
}

inline double RC4Cipher::CalculateKeyHash() const {
  const auto N = N_;

  u32 hash = 1;
  for (u32 i = 0; i < N_; i++) {
    auto value = i32{key_[i]};

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
  auto& S = S_;
  const auto N = N_;
  S.resize(N);

  for (u32 i = 0; i < N; i++) {
    S[i] = i & 0xFF;
  }

  u32 j = 0;
  for (u32 i = 0; i < N; i++) {
    j = (S[i] + j + key_[i % N]) % N;
    std::swap(S[i], S[j]);
  }
}

inline u64 RC4Cipher::GetSegmentKey(u64 segment_id, u64 seed) const {
  return u64(key_hash_ / double((segment_id + 1) * seed) * 100.0);
}

__umc_fi void RC4Cipher::EncodeFirstSegment(u8* out) {
  for (usize i = 0; i < FIRST_SEGMENT_SIZE; i++) {
    const u64 seed = u64{key_[i % N_]};
    out[i] = key_[GetSegmentKey(i, seed) % N_];
  }
}

__umc_fi void RC4Cipher::EncodeOtherSegment(u8* out) {
  const auto N = N_;
  Vec<u8> S(this->S_);

  size_t segment_seed = key_[segment_id_ & 0x1FF];
  auto discards = GetSegmentKey(segment_id_, segment_seed) & 0x1FF;

  u32 j = 0;
  u32 k = 0;
  for (u32 i = 0; i < discards; i++) {
    j = (j + 1) % N;
    k = (S[j] + k) % N;
    std::swap(S[j], S[k]);
  }

  // Now we manipulate the buffer.
  for (u32 i = 0; i < OTHER_SEGMENT_SIZE; i++) {
    j = (j + 1) % N;
    k = (S[j] + k) % N;
    std::swap(S[j], S[k]);

    out[i] = S[(S[j] + S[k]) % N];
  }

  segment_id_++;
}

void RC4Cipher::HardSeek(usize offset) {
  AXorStreamCipher::HardSeek(offset);

  segment_id_ = offset / OTHER_SEGMENT_SIZE;
  buf_idx_ = offset % OTHER_SEGMENT_SIZE;

  EncodeOtherSegment(buf_.data());

  if (segment_id_ == 1) {
    EncodeFirstSegment(buf_.data());
  }
}

void RC4Cipher::YieldNextXorBuf(Vec<u8>& buf) {
  buf_idx_ = 0;
  EncodeOtherSegment(buf.data());
}
