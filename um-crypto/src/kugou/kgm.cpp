#include "kgm_crypto.h"
#include "um-crypto/kugou.h"

using namespace umc::kugou;

bool KGMCipher::Decrypt(Vec<u8>& result, const Vec<u8>& input) {
  auto len = input.size();
  result.resize(len);

  const auto file_key = file_key_.data();
  for (usize i = 0; i < len; i++) {
    result[i] = DecryptKGMV2(offset, input[i], file_key);
  }

  return true;
}

bool KGMCipher::Encrypt(Vec<u8>& result, const Vec<u8>& input) {
  return Decrypt(result, input);
}
