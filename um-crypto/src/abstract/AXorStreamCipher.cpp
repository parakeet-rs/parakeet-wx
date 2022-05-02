#include "um-crypto/abstract/AXorStreamCipher.h"
#include "um-crypto/common.h"

#include <algorithm>

using namespace umc;

void AXorStreamCipher::Seek(usize new_offset) {
  isize new_buf_index = isize(new_offset) - isize(offset_) + buf_idx_;

  // We seek outside of current buffer - do a hard seek.
  if (new_buf_index < 0 || new_buf_index > isize(buf_.size())) {
    buf_idx_ = 0;
    HardSeek(new_offset);
    return;
  }

  // Soft seek - we are still within the same block.
  buf_idx_ = new_buf_index;
  offset_ = new_offset;
}

inline void XorBlock(u8* p_out, const u8* p_in, const u8* key, usize len) {
  for (usize i = 0; i < len; i++) {
    p_out[i] = p_in[i] ^ key[i];
  }
}

bool AXorStreamCipher::Decrypt(u8* p_out,
                               usize& out_len,
                               const u8* p_in,
                               usize in_len) {
  // insufficient size.
  if (out_len < in_len) {
    out_len = in_len;
    return false;
  }

  usize len = out_len = in_len;

  while (len > 0) {
    usize bytes_available = buf_.size() - buf_idx_;

    // Check if we need to yield a new block.
    if (bytes_available == 0) {
      YieldNextXorBuf(buf_);
      bytes_available = buf_.size() - buf_idx_;

      // If we don't have anything available, there's something wrong.
      if (bytes_available == 0) {
        return false;
      }
    }

    auto processed_len = std::min(len, bytes_available);
    XorBlock(p_out, p_in, &buf_[buf_idx_], processed_len);

    len -= processed_len;
    offset_ += processed_len;
    buf_idx_ += processed_len;

    p_out += processed_len;
    p_in += processed_len;
  }

  return true;
}
