#pragma once
#include "um-crypto/common.h"

namespace umc {

class IStreamCipher {
 public:
  IStreamCipher(){};
  virtual ~IStreamCipher(){};

  virtual bool Encrypt(Vec<u8>& result, const Vec<u8>& input) = 0;
  virtual bool Decrypt(Vec<u8>& result, const Vec<u8>& input) = 0;
  virtual void Seek(usize offset) { this->offset = offset; }

 protected:
  usize offset = 0;
};

}  // namespace umc
