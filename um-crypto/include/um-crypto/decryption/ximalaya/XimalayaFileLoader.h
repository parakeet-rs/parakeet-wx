#pragma once

#include "../DecryptionStream.h"

namespace umc::decryption::ximalaya {

constexpr usize kX2MContentKeySize = 0x04;
constexpr usize kX3MContentKeySize = 0x20;
constexpr usize kScrambleTableSize = 0x400;

typedef Arr<u8, kX2MContentKeySize> X2MContentKey;
typedef Arr<u8, kX3MContentKeySize> X3MContentKey;
typedef Arr<u16, kScrambleTableSize> ScrambleTable;

class XimalayaFileLoader : public DecryptionStream {
 public:
  virtual const Str GetName() const override { return "Ximalaya"; };

  static std::unique_ptr<XimalayaFileLoader> Create(
      const X2MContentKey& key,
      const ScrambleTable& scramble_table);

  static std::unique_ptr<XimalayaFileLoader> Create(
      const X3MContentKey& key,
      const ScrambleTable& scramble_table);
};

}  // namespace umc::decryption::ximalaya
