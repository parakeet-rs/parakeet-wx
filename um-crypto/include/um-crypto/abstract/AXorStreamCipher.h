#pragma once
#include "um-crypto/interface/IStreamCipher.h"

#include <deque>

namespace umc {

class AXorStreamCipher : public IStreamCipher {
 public:
  AXorStreamCipher(){};
  virtual ~AXorStreamCipher(){};

  virtual bool Encrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) override {
    return Decrypt(p_out, out_len, p_in, in_len);
  }

  virtual bool Decrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) override;

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
