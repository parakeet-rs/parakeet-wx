#pragma once
#include "AXimalayaAndroidHeaderCipher.h"

#include "um-crypto/abstract/AXorStreamCipher.h"
#include "um-crypto/types.h"

namespace umc::ximalaya {

class X3MCipher : public AXimalayaAndroidHeaderCipher {
 public:
  X3MCipher(const XimalayaAndroidFileHeader& header);

 private:
  const static XimalayaHeaderScrambleTable scramble_table_;
  XimalayaAndroidFileHeader header_;
};

}  // namespace umc::ximalaya
