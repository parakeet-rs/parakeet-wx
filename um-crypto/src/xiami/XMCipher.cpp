#include "um-crypto/xiami/XMCipher.h"

namespace umc::xiami {

XMCipher::XMCipher(XiamiXMConfig& config) : IStreamCipher() {
  file_key_ = config.file_key - 1;
  noop_len_ = config.noop_len;
}

bool XMCipher::Encrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) {
  if (out_len < in_len) {
    out_len = in_len;
    return false;
  }
  out_len = in_len;

  usize start = 0;
  if (noop_len_ > offset_) {
    const usize len = noop_len_ - offset_;
    std::copy(p_in, &p_in[len], p_out);

    offset_ += len;
    start = len;
  }

  for (usize i = start; i < in_len; i++) {
    p_out[i] = file_key_ + p_in[i];
  }

  return true;
}

bool XMCipher::Decrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) {
  if (out_len < in_len) {
    out_len = in_len;
    return false;
  }
  out_len = in_len;

  usize start = 0;
  if (noop_len_ > offset_) {
    const usize len = std::min(noop_len_ - offset_, in_len);
    std::copy(p_in, &p_in[len], p_out);

    offset_ += len;
    start = len;
  }

  for (usize i = start; i < in_len; i++) {
    p_out[i] = file_key_ - p_in[i];
  }

  return true;
}

}  // namespace umc::xiami
