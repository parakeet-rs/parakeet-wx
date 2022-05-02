#pragma once

#include "um-crypto/interface/IStreamCipher.h"

namespace umc::kugou {

class KGMCipher : public IStreamCipher {
 public:
  KGMCipher(const Vec<u8>& file_key) : IStreamCipher() { file_key_ = file_key; }
  virtual ~KGMCipher() {}

  bool Encrypt(Vec<u8>& result, const Vec<u8>& input) override;
  bool Decrypt(Vec<u8>& result, const Vec<u8>& input) override;

 private:
  Vec<u8> file_key_;
};

}  // namespace umc::kugou
