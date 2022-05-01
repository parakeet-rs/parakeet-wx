#pragma once

#include "um-crypto/abstract/XorStreamCipherBase.h"
#include "um-crypto/common.h"
#include "um-crypto/interface/IStreamCipher.h"

namespace umc::qmcv2 {

class MapCipher : public XorStreamCipherBase {
 public:
  MapCipher(const Vec<u8>& key);
  MapCipher(const u8* key, const usize key_len)
      : MapCipher(Vec<u8>(key, key + key_len)){};

  void Seek(usize offset) override;

 protected:
  void YieldNextXorBuf(Vec<u8>& buf) override;

 private:
  Vec<u8> table;
  std::pair<u8, u8> first_bytes;
};

}  // namespace umc::qmcv2
