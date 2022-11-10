#pragma once

#include "um-crypto/decryption/DecryptionStream.h"

#include <array>
#include <memory>
#include <string>

#include <cstdint>

namespace umc::decryption::netease {

// AES Key; which can be used to decrypt the embedded "content key"
constexpr std::size_t kNCMContentKeyProtectionKeySize = 128 / 8;
typedef std::array<uint8_t, kNCMContentKeyProtectionKeySize> NCMContentKeyProtectionKey;

class NCMFileLoader : public DecryptionStream {
 public:
  virtual const std::string GetName() const override { return "NCM"; };

  static std::unique_ptr<NCMFileLoader> Create(const NCMContentKeyProtectionKey& key);
};

}  // namespace umc::decryption::netease
