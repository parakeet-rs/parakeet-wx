#pragma once
#include "common.h"

#include "um-crypto/interface/IStreamCipher.h"

namespace umc::xiami {

class XMCipher : public IStreamCipher {
 public:
  XMCipher(XiamiXMConfig& config);

  using IStreamCipher::Decrypt;
  using IStreamCipher::Encrypt;

  virtual bool Encrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) override;

  virtual bool Decrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) override;

 private:
  u8 file_key_;
  usize noop_len_;
};

}  // namespace umc::xiami
