#include "um-crypto/kugou/KGMCipher.h"

#include "../internal/XorHelper.h"
#include "um-crypto/kugou/KGMMaskGenerator.h"
#include "um-crypto/kugou/constants.h"

#include <cstring>

namespace umc::kugou {

constexpr usize kPageSize = 4 * 1024 * 1024;

KGMCipher::KGMCipher(const KugouFileKey& file_key)
    : KGMCipher(file_key, KGMMaskGenerator::GetInstance()){};

KGMCipher::KGMCipher(const KugouFileKey& file_key,
                     KGMMaskGenerator::Ptr mask_generator)
    : AXorStreamCipher(), file_key_(file_key), mask_generator_(mask_generator) {
  buf_.resize(kPageSize);
  buf_idx_ = kPageSize;
}

void KGMCipher::PostProcess(u8* p_out, usize len) {
  for (usize i = 0; i < len; i++) {
    p_out[i] ^= p_out[i] << 4;
  }
}

void KGMCipher::YieldNextXorBuf(Vec<u8>& buf) {
  buf_idx_ = 0;

  KGMMaskGenerator::GetInstance()->FetchMask(buf_.data(), kPageSize, offset_);
  XorBlock(buf_.data(), buf.size(), file_key_.data(), kFileKeySize, offset_);
}

}  // namespace umc::kugou
