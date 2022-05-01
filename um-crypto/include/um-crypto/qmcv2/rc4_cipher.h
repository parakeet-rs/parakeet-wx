#pragma once

#include "um-crypto/common.h"
#include "um-crypto/interface/IStreamCipher.h"

namespace umc::qmcv2 {

class RC4Cipher : public ::umc::IStreamCipher {
 public:
  RC4Cipher(const Vec<u8>& key);
  RC4Cipher(const u8* key, const usize key_len)
      : RC4Cipher(Vec<u8>(key, key + key_len)){};
  ~RC4Cipher(){};

  virtual bool Encrypt(Vec<u8>& result, const Vec<u8>& input) override;
  virtual bool Decrypt(Vec<u8>& result, const Vec<u8>& input) override;

 private:
  Vec<u8> S;
  u32 N;  // S.size()

  Vec<u8> key;
  double key_hash;

  void InitSeedbox();
  double CalculateKeyHash() const;
  u64 GetSegmentKey(usize segment_id, u64 seed) const;
  void EncodeFirstSegment(u8* out, const u8* buf, usize len);
  void EncodeOtherSegment(u8* out, const u8* buf, usize len);
};

}  // namespace umc::qmcv2
