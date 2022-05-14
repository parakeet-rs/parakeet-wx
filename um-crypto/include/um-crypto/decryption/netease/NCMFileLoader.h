#pragma once

#include "../DecryptionStream.h"

namespace umc::decryption::netease {

// AES Key; which can be used to decrypt the embedded "content key"
constexpr usize kNCMContentKeyProtectionKeySize = 128 / 8;
typedef Arr<u8, kNCMContentKeyProtectionKeySize> NCMContentKeyProtectionKey;

class NCMFileLoader : public DecryptionStream {
 public:
  virtual const Str GetName() const override { return "NCM"; };

  static std::unique_ptr<NCMFileLoader> Create(
      const NCMContentKeyProtectionKey& key);
};

}  // namespace umc::decryption::netease
