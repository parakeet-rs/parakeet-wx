#include "um-crypto/qmcv2/map_cipher.h"

using namespace umc;
using namespace umc::qmcv2;

inline u8 Rotate(u8 value, usize bits) {
  usize shift_amount = (bits + 4) % 8;
  auto left = value << shift_amount;
  auto right = value >> shift_amount;
  return u8(left | right);
}

inline u8 GetMaskByte(const u8* key, usize n, usize offset) {
  u64 idx = (offset * offset + 71214) % n;
  u8 value = key[idx];

  // Rotate by the lower 3 bits
  return Rotate(value, idx & 0b0111);
}

constexpr usize SEGMENT_LENGTH = 0x7fff;

MapCipher::MapCipher(const Vec<u8>& key) : XorStreamCipherBase() {
  const usize N = key.size();
  buf.resize(SEGMENT_LENGTH);

  const u8* p_key = key.data();
  for (usize i = 1; i < SEGMENT_LENGTH; i++) {
    buf[i] = GetMaskByte(p_key, N, offset + i);
  }
  first_bytes.first = GetMaskByte(p_key, N, 0);
  first_bytes.second = GetMaskByte(p_key, N, SEGMENT_LENGTH);
}

void MapCipher::YieldNextXorBuf(Vec<u8>& buf) {
  buf_idx_ = offset % SEGMENT_LENGTH;
  buf[0] = offset == SEGMENT_LENGTH ? first_bytes.second : first_bytes.first;
}

void MapCipher::Seek(usize offset) {
  XorStreamCipherBase::Seek(offset);
  // invalidate cache without losing data
  buf_idx_ = SEGMENT_LENGTH;
}
