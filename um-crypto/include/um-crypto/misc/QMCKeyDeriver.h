#pragma once

#include "um-crypto/types.h"

namespace umc::misc::tencent {

class QMCKeyDeriver {
 public:
  virtual bool FromEKey(Vec<u8>& out, const Str ekey_b64) const = 0;
  virtual bool FromEKey(Vec<u8>& out, const Vec<u8> ekey) const = 0;
  virtual bool ToEKey(Vec<u8>& out, const Vec<u8> key) const = 0;

  static std::unique_ptr<QMCKeyDeriver> Create(u8 seed);
};

}  // namespace umc::misc::tencent
