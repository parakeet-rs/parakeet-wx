#include "um-crypto/qmcv2/map_cipher.h"

using namespace umc;
using namespace umc::qmcv2;

const usize kSegmentLen = 0x7fff;
const usize kDoubleSegmentLen = kSegmentLen * 2;

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

MapCipher::MapCipher(const Vec<u8>& key) : AXorStreamCipher() {
  const usize N = key.size();
  buf_.resize(kSegmentLen);

  u8* buf = buf_.data();
  const u8* p_key = key.data();
  for (usize i = 0; i < kSegmentLen; i++) {
    buf[i] = GetMaskByte(p_key, N, offset_ + i);
  }
  first_bytes.first = buf[0];
  first_bytes.second = GetMaskByte(p_key, N, kSegmentLen);
}

void MapCipher::YieldNextXorBuf(Vec<u8>& buf) {
  buf_idx_ = offset_ % kSegmentLen;

  // Check if we are in the second segment, which uses a different value
  //   for the first indexed item.
  buf[0] = (offset_ >= kSegmentLen && offset_ < kDoubleSegmentLen)
               ? first_bytes.second
               : first_bytes.first;
}
