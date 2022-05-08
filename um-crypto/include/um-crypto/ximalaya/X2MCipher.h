#pragma once
#include "AXimalayaAndroidHeaderCipher.h"

#include "um-crypto/abstract/AXorStreamCipher.h"
#include "um-crypto/types.h"

namespace umc::ximalaya {

class X2MCipher : public AXimalayaAndroidHeaderCipher {
 public:
  X2MCipher(const XimalayaAndroidFileHeader& header);
  X2MCipher(const XimalayaAndroidFileHeader& header,
            const XimalayaX2MContentKey& content_key,
            const XimalayaHeaderScrambleTable& scramble_table);

  static void SetScrambleTable(const XimalayaHeaderScrambleTable& table);
  static void SetContentKey(const XimalayaX2MContentKey& key);

 private:
  static XimalayaHeaderScrambleTable scramble_table_;
  static XimalayaX2MContentKey content_key_;
  XimalayaAndroidFileHeader header_;
};

}  // namespace umc::ximalaya
