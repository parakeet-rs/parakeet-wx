#pragma once
#include "um-crypto/interface/IStreamCipher.h"

#include <deque>

namespace umc {

class XorStreamCipherBase : public IStreamCipher {
 public:
  XorStreamCipherBase(){};
  virtual ~XorStreamCipherBase(){};

  virtual bool Encrypt(Vec<u8>& result, const Vec<u8>& input) override {
    return XorStream(result, input);
  };

  virtual bool Decrypt(Vec<u8>& result, const Vec<u8>& input) override {
    return XorStream(result, input);
  };

  bool XorStream(Vec<u8>& result, const Vec<u8>& input);

 protected:
  virtual void YieldNextXorBuf(Vec<u8>& buf) = 0;
  Vec<u8> buf;
  usize buf_idx_ = 0;
};

}  // namespace umc
