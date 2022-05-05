#include "um-crypto/ximalaya/AXimalayaAndroidHeaderCipher.h"

#include <algorithm>

namespace umc::ximalaya {

AXimalayaAndroidHeaderCipher::AXimalayaAndroidHeaderCipher(
    const XimalayaAndroidFileHeader& header,
    const XimalayaHeaderContentKey& content_key,
    const XimalayaHeaderScrambleTable& scramble_table)
    : IStreamCipher(),
      header_(header),
      content_key_(content_key),
      scramble_table_(scramble_table) {}

bool AXimalayaAndroidHeaderCipher::Decrypt(u8* p_out,
                                           usize& out_len,
                                           const u8* p_in,
                                           usize in_len) {
  if (out_len < in_len) {
    out_len = in_len;
    return false;
  }
  out_len = in_len;

  usize offset = offset_;
  if (offset < kXimalayaEncryptedHeaderSize) {
    usize len = std::min(in_len, kXimalayaEncryptedHeaderSize - offset);
    for (usize i = 0; i < len; i++) {
      *p_out++ = header_[static_cast<usize>(scramble_table_[offset])] ^
                 content_key_[offset & kXimalayaHeaderContentKeyIndexMask];
      offset++;
    }
    p_in += len;
    in_len -= len;
    offset_ = offset;
  }

  if (in_len > 0) {
    offset_ += in_len;
    std::copy(p_in, p_in + in_len, p_out);
  }
  return true;
}

}  // namespace umc::ximalaya
