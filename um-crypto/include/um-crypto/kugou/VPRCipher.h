#pragma once

#include "KGMCipher.h"

namespace umc::kugou {

class VPRCipher : public KGMCipher {
 public:
  VPRCipher(const KugouFileKey& file_key, const KugouFileKey& vpr_key);
  virtual ~VPRCipher() {}
  virtual void PostProcess(u8* p_out, usize len) override;

 protected:
  KugouFileKey vpr_key_;
};

}  // namespace umc::kugou
