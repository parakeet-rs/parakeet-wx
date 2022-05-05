#include "um-crypto/kuwo/KWMCipher.h"

#include "../internal/XorHelper.h"

#include <algorithm>
#include <sstream>

namespace umc::kuwo {

const u8 kKuwoDecryptionKey[32] = {
    0x4d, 0x6f, 0x4f, 0x74, 0x4f, 0x69, 0x54, 0x76, 0x49, 0x4e, 0x47,
    0x77, 0x64, 0x32, 0x45, 0x36, 0x6e, 0x30, 0x45, 0x31, 0x69, 0x37,
    0x4c, 0x35, 0x74, 0x32, 0x49, 0x6f, 0x4f, 0x6f, 0x4e, 0x6b,
};

KWMCipher::KWMCipher(u64 file_key) : AXorStreamCipher() {
  std::stringstream ss;
  ss << file_key;
  auto file_key_str = ss.str();
  auto const p_initial_file_key = reinterpret_cast<u8*>(file_key_str.data());

  // Populate file key block
  Arr<u8, kKuwoKeySize> file_key_block;
  std::copy(kKuwoDecryptionKey, &kKuwoDecryptionKey[kKuwoKeySize],
            file_key_block.data());

  // xor our key.
  XorBlock(file_key_block.data(), kKuwoKeySize, p_initial_file_key,
           file_key_str.size(), 0);

  // Copy key to our cache.
  buf_.resize(kKuwoBufSize);
  auto p_buf = buf_.data();
  for (usize i = 0; i < kKuwoBufSize; i += kKuwoKeySize) {
    std::copy(file_key_block.begin(), file_key_block.end(), &p_buf[i]);
  }
}

void KWMCipher::YieldNextXorBuf(Vec<u8>& buf) {
  buf_idx_ = offset_ % kKuwoKeySize;
}

}  // namespace umc::kuwo
