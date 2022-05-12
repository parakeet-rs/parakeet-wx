#pragma once

#include "../DecryptionStream.h"

namespace umc::decryption::kuwo {

constexpr usize kKuwoDecryptionKeySize = 0x20;
typedef Arr<u8, kKuwoDecryptionKeySize> KuwoKey;

class KuwoFileLoader : public DecryptionStream {
 public:
  virtual const Str GetName() const override { return "Kuwo"; };

  static std::unique_ptr<KuwoFileLoader> Create(const KuwoKey& key);
};

}  // namespace umc::decryption::kuwo
