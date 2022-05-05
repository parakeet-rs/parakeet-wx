#pragma once
#include "AXimalayaAndroidHeaderCipher.h"

#include "um-crypto/abstract/AXorStreamCipher.h"
#include "um-crypto/types.h"

namespace umc::ximalaya {

class X2MCipher : public AXimalayaAndroidHeaderCipher {
 public:
  X2MCipher(const XimalayaAndroidFileHeader& header);

 private:
  const static XimalayaHeaderScrambleTable scramble_table_;
  XimalayaAndroidFileHeader header_;
};

}  // namespace umc::ximalaya
