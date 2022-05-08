#pragma once

#include "um-crypto/abstract/AXorStreamCipher.h"

#include "um-crypto/types.h"

namespace umc::tencent {

typedef Arr<u8, 128> StaticCipherKey;

class StaticStreamCipher : public AXorStreamCipher {
 public:
  StaticStreamCipher();
  StaticStreamCipher(const StaticCipherKey& key);

  static void SetStaticKey(const StaticCipherKey& key) { static_key_ = key; }

 protected:
  const StaticCipherKey& key_;
  void YieldNextXorBuf(Vec<u8>& buf) override;

  static StaticCipherKey static_key_;
};

}  // namespace umc::tencent
