#pragma once
#include <cassert>
#include "um-crypto/types.h"

namespace umc {

class IStreamCipher {
 public:
  IStreamCipher(){};
  virtual ~IStreamCipher(){};

  /**
   * @brief Encrypt a stream of data.
   * It will automatically retry once on insufficient output size.
   *
   * @param p_out pointer to output buffer
   * @param out_len output buffer size
   * @param p_in pointer to input buffer
   * @param in_len input buffer size
   * @return true Encryption completed successfully.
   * @return false
   */
  virtual bool Encrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) = 0;
  virtual bool Decrypt(u8* p_out,
                       usize& out_len,
                       const u8* p_in,
                       usize in_len) = 0;

  // Dynamic output resize wrapper
  virtual bool Encrypt(Vec<u8>& out, const u8* p_in, usize in_len) {
    usize initial_out_len = out.size();
    usize out_len = initial_out_len;
    bool ok = Encrypt(out.data(), out_len, p_in, in_len);
    out.resize(out_len);
    if (!ok && out_len > initial_out_len) {
      ok = Encrypt(out.data(), out_len, p_in, in_len);
      out.resize(out_len);
    }
    return ok;
  }
  virtual bool Decrypt(Vec<u8>& out, const u8* p_in, usize in_len) {
    usize initial_out_len = out.size();
    usize out_len = initial_out_len;
    bool ok = Decrypt(out.data(), out_len, p_in, in_len);
    out.resize(out_len);
    if (!ok && out_len > initial_out_len) {
      ok = Decrypt(out.data(), out_len, p_in, in_len);
      out.resize(out_len);
    }
    return ok;
  }

  // Simple wrappers.
  virtual bool Encrypt(Vec<u8>& out, const Vec<u8>& in) {
    return Encrypt(out, in.data(), in.size());
  }
  virtual bool Decrypt(Vec<u8>& out, const Vec<u8>& in) {
    return Decrypt(out, in.data(), in.size());
  }
  virtual bool Encrypt(u8* p_out, usize& out_len, const Vec<u8>& in) {
    return Encrypt(p_out, out_len, in.data(), in.size());
  }
  virtual bool Decrypt(u8* p_out, usize& out_len, const Vec<u8>& in) {
    return Decrypt(p_out, out_len, in.data(), in.size());
  }

  virtual void Seek(usize offset) { this->offset_ = offset; }

 protected:
  usize offset_ = 0;
};

}  // namespace umc
