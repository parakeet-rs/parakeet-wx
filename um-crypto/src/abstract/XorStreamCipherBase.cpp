#include "um-crypto/abstract/XorStreamCipherBase.h"
#include "um-crypto/common.h"

#include <algorithm>

using namespace umc;

inline void XorBlock(u8* out, const u8* in, const u8* key, usize len) {
  for (usize i = 0; i < len; i++) {
    out[i] = in[i] ^ key[i];
  }
}

bool XorStreamCipherBase::XorStream(u8* result, const u8* input, usize len) {
  if (buf_idx_ >= buf.size() || buf.empty()) {
    buf_idx_ = 0;
    YieldNextXorBuf(buf);
  }

  while (len > 0) {
    auto buf_size = buf.size() - buf_idx_;
    auto processed_len = std::min(len, buf_size);
    XorBlock(result, input, &buf[buf_idx_], processed_len);

    len -= processed_len;
    offset += processed_len;
    buf_idx_ += processed_len;

    result += processed_len;
    input += processed_len;

    if (buf_size == processed_len) {
      buf_idx_ = 0;
      YieldNextXorBuf(buf);
    }
  }

  return true;
}

bool XorStreamCipherBase::XorStream(Vec<u8>& result, const Vec<u8>& input) {
  usize len = input.size();
  result.resize(len);
  return XorStream(result.data(), input.data(), len);
}
