#pragma once
#include "um-crypto/types.h"

#include "um-crypto/abstract/AXorStreamCipher.h"

namespace umc::kuwo {
constexpr usize kKuwoKeySize = 32;
constexpr usize kKuwoBufSize = 4096;

static_assert((kKuwoBufSize % kKuwoKeySize) == 0,
              "kuwo buf size should be multiple of 32");

class KWMCipher : public AXorStreamCipher {
 public:
  KWMCipher(u64 file_key);
  virtual ~KWMCipher() {}

  void YieldNextXorBuf(Vec<u8>& buf) override;
};

}  // namespace umc::kuwo
