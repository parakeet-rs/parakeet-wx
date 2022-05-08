#include "um-crypto/tencent/StaticStreamCipher.h"

namespace umc::tencent {

constexpr usize kStaticCipherPageSize = 0x7fff;

StaticCipherKey StaticStreamCipher::static_key_;

StaticStreamCipher::StaticStreamCipher(const StaticCipherKey& key)
    : AXorStreamCipher(), key_(key) {
  buf_.resize(kStaticCipherPageSize);
  buf_idx_ = kStaticCipherPageSize;
}

StaticStreamCipher::StaticStreamCipher() : StaticStreamCipher(static_key_) {}

void StaticStreamCipher::YieldNextXorBuf(Vec<u8>& buf) {
  buf_idx_ = 0;

  usize i = offset_;
  usize end = offset_ + kStaticCipherPageSize;
  u8* p_buf = buf.data();

  for (; i < end; i++) {
    *p_buf = key_[(i % 0x7FFF) & 0x7F];
    p_buf++;
  }
}

};  // namespace umc::tencent
