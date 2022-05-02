#pragma once

#include "um-crypto/interface/IStreamCipher.h"

namespace umc::kugou {

class KGMCipher : public IStreamCipher {
 public:
  KGMCipher(const Vec<u8>& file_key) : IStreamCipher() { file_key_ = file_key; }
  virtual ~KGMCipher() {}

  virtual bool Encrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) override;
  virtual bool Decrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) override;

 private:
  Vec<u8> file_key_;
};

}  // namespace umc::kugou
