#pragma once
#include "common.h"

#include "um-crypto/abstract/AXorStreamCipher.h"
#include "um-crypto/types.h"

namespace umc::ximalaya {

class AXimalayaAndroidHeaderCipher : public IStreamCipher {
 public:
  AXimalayaAndroidHeaderCipher(
      const XimalayaAndroidFileHeader& header,
      const XimalayaHeaderContentKey& content_key,
      const XimalayaHeaderScrambleTable& scramble_table);

  using IStreamCipher::Decrypt;
  using IStreamCipher::Encrypt;

  virtual bool Encrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) override {
    // un-implemented
    return false;
  }

  virtual bool Decrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) override;

 protected:
  const XimalayaHeaderContentKey& content_key_;
  const XimalayaHeaderScrambleTable& scramble_table_;

 private:
  const XimalayaAndroidFileHeader header_;
};

}  // namespace umc::ximalaya
