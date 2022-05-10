#pragma once

#include "../DecryptionStream.h"

namespace umc::decryption::tencent {

typedef Arr<u8, 16> JooxSalt;

class JooxFileLoader : public DecryptionStream {
 public:
  virtual Str GetName() override { return "joox"; };

  static std::unique_ptr<JooxFileLoader> Create(const Str& install_uuid,
                                                const JooxSalt& salt);
};

}  // namespace umc::decryption::tencent
