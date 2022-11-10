#pragma once

#include "um-crypto/decryption/DecryptionStream.h"

namespace umc::decryption::netease {

// AES Key; which can be used to decrypt the embedded "content key"
constexpr std::size_t kNCMContentKeyProtectionKeySize = 128 / 8;
typedef std::array<u8, kNCMContentKeyProtectionKeySize>
    NCMContentKeyProtectionKey;

class NCMFileLoader : public DecryptionStream {
 public:
  virtual const std::string GetName() const override { return "NCM"; };

  static std::unique_ptr<NCMFileLoader> Create(
      const NCMContentKeyProtectionKey& key);
};

}  // namespace umc::decryption::netease
