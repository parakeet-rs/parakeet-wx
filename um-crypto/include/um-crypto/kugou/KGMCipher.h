#pragma once

#include "KGMMaskGenerator.h"
#include "constants.h"
#include "um-crypto/abstract/AXorStreamCipher.h"

namespace umc::kugou {

class KGMCipher : public AXorStreamCipher {
 public:
  KGMCipher(const KugouFileKey& file_key);
  KGMCipher(const KugouFileKey& file_key, KGMMaskGenerator::Ptr mask_generator);
  virtual ~KGMCipher() {}

  void YieldNextXorBuf(Vec<u8>& buf) override;
  virtual void PostProcess(u8* p_out, usize len) override;

 protected:
  KugouFileKey file_key_;
  KGMMaskGenerator::Ptr mask_generator_;
};

}  // namespace umc::kugou
