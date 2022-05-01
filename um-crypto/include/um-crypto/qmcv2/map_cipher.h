#pragma once

#include "um-crypto/common.h"
#include "um-crypto/interface/IStreamCipher.h"

namespace umc::qmcv2 {

class MapCipher : public ::umc::IStreamCipher {
 public:
  MapCipher(const Vec<u8>& key);
  MapCipher(const u8* key, const usize key_len)
      : MapCipher(Vec<u8>(key, key + key_len)){};
  ~MapCipher(){};

  virtual bool Encrypt(Vec<u8>& result, const Vec<u8>& input) override;
  virtual bool Decrypt(Vec<u8>& result, const Vec<u8>& input) override;

 private:
  Vec<u8> key;
  u32 N;

  u8 GetMaskByte();
};

}  // namespace umc::qmcv2
