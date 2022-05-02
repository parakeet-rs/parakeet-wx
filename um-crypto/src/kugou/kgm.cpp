#include "kgm_crypto.h"
#include "um-crypto/kugou.h"

using namespace umc::kugou;

bool KGMCipher::Decrypt(u8* p_out,
                        usize& out_len,
                        const u8* p_in,
                        usize in_len) {
  if (in_len > out_len) {
    out_len = in_len;
    return false;
  }
  out_len = in_len;

  const auto file_key = file_key_.data();
  for (usize i = 0; i < in_len; i++) {
    p_out[i] = DecryptKGMV2(offset_, p_in[i], file_key);
    offset_++;
  }

  return true;
}

bool KGMCipher::Encrypt(u8* p_out,
                        usize& out_len,
                        const u8* p_in,
                        usize in_len) {
  return Decrypt(p_out, out_len, p_in, in_len);
}
