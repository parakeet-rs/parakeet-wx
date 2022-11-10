#pragma once

#include "um-crypto/decryption/DecryptionStream.h"

namespace umc::decryption::kuwo {

constexpr std::size_t kKuwoDecryptionKeySize = 0x20;
typedef std::array<u8, kKuwoDecryptionKeySize> KuwoKey;

class KuwoFileLoader : public DecryptionStream {
 public:
  virtual const std::string GetName() const override { return "Kuwo"; };

  static std::unique_ptr<KuwoFileLoader> Create(const KuwoKey& key);
};

}  // namespace umc::decryption::kuwo
