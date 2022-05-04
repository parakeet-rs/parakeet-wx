#include "um-crypto/kugou/VPRCipher.h"

#include "../internal/XorHelper.h"

#include <cstring>

namespace umc::kugou {

constexpr usize kPageSize = 4096;

VPRCipher::VPRCipher(const KugouFileKey& file_key, const KugouFileKey& vpr_key)
    : KGMCipher(file_key) {
  vpr_key_ = vpr_key;
}

void VPRCipher::PostProcess(u8* p_out, usize len) {
  KGMCipher::PostProcess(p_out, len);
  XorBlock(p_out, len, vpr_key_.data(), kFileKeySize, offset_);
}

}  // namespace umc::kugou
