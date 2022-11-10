#pragma once

#include "um-crypto/decryption/DecryptionStream.h"

namespace umc::decryption::xiami {

class XiamiFileLoader : public DecryptionStream {
 public:
  virtual const std::string GetName() const override { return "Xiami"; };
  static std::unique_ptr<XiamiFileLoader> Create();
};

}  // namespace umc::decryption::xiami
