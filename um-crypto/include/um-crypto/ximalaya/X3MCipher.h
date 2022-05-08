#pragma once
#include "AXimalayaAndroidHeaderCipher.h"

#include "um-crypto/abstract/AXorStreamCipher.h"
#include "um-crypto/types.h"

namespace umc::ximalaya {

class X3MCipher : public AXimalayaAndroidHeaderCipher {
 public:
  X3MCipher(const XimalayaAndroidFileHeader& header);
  X3MCipher(const XimalayaAndroidFileHeader& header,
            const XimalayaHeaderContentKey& content_key,
            const XimalayaHeaderScrambleTable& scramble_table);

  static void SetScrambleTable(const XimalayaHeaderScrambleTable& table);
  static void SetContentKey(const XimalayaHeaderContentKey& key);

 private:
  static XimalayaHeaderScrambleTable scramble_table_;
  static XimalayaHeaderContentKey content_key_;
  XimalayaAndroidFileHeader header_;
};

}  // namespace umc::ximalaya
