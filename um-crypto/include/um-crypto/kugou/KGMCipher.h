#pragma once

#include "constants.h"
#include "um-crypto/abstract/AXorStreamCipher.h"

namespace umc::kugou {

class KGMCipher : public AXorStreamCipher {
 public:
  KGMCipher(const KugouFileKey& file_key);
  virtual ~KGMCipher() {}

  void YieldNextXorBuf(Vec<u8>& buf) override;
  virtual void PostProcess(u8* p_out, usize len) override;

 protected:
  KugouFileKey file_key_;
};

}  // namespace umc::kugou
