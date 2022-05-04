#pragma once

#include "um-crypto/abstract/AXorStreamCipher.h"

#include "um-crypto/types.h"

namespace umc::tencent {

class StaticStreamCipher : public AXorStreamCipher {
 public:
  StaticStreamCipher();

 protected:
  void YieldNextXorBuf(Vec<u8>& buf) override;
};

}  // namespace umc::tencent
