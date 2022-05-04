#pragma once
#include "common.h"

#include "um-crypto/abstract/AXorStreamCipher.h"
#include "um-crypto/types.h"

#include <array>

namespace umc::ximalaya {

class X2MCipher : public IStreamCipher {
 public:
  X2MCipher(const X2MHeader& header);

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

 private:
  const static X2MScrambleTable scramble_table_;
  X2MHeader header_;
};

}  // namespace umc::ximalaya
