#pragma once
#include "um-crypto/interface/IStreamCipher.h"

#include <deque>

namespace umc {

class AXorStreamCipher : public IStreamCipher {
 public:
  AXorStreamCipher(){};
  virtual ~AXorStreamCipher(){};

  virtual bool Encrypt(Vec<u8>& result, const Vec<u8>& input) override {
    return XorStream(result, input);
  };

  virtual bool Decrypt(Vec<u8>& result, const Vec<u8>& input) override {
    return XorStream(result, input);
  };

  bool XorStream(Vec<u8>& result, const Vec<u8>& input);
  bool XorStream(u8* result, const u8* input, usize len);

  // Fast seek, when the user seeks within the same block.
  void Seek(usize offset) override;
  virtual void HardSeek(usize offset) {
    offset_ = offset;
    buf_idx_ = buf_.size();
  };

 protected:
  virtual void YieldNextXorBuf(Vec<u8>& buf) = 0;
  Vec<u8> buf_;
  usize buf_idx_ = 0;
};

}  // namespace umc
