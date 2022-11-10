#pragma once

#include "um-crypto/decryption/DecryptionStream.h"

namespace umc::decryption::tencent {

typedef std::array<u8, 16> JooxSalt;

class JooxFileLoader : public DecryptionStream {
 public:
  virtual const std::string GetName() const override { return "joox"; };

  static std::unique_ptr<JooxFileLoader> Create(const std::string& install_uuid,
                                                const JooxSalt& salt);
};

}  // namespace umc::decryption::tencent
