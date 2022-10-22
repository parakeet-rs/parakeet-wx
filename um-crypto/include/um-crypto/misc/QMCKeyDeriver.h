#pragma once

#include "um-crypto/types.h"

namespace umc::misc::tencent {
typedef Arr<u8, 16> QMCEncV2Stage1Key;
typedef Arr<u8, 16> QMCEncV2Stage2Key;

class QMCKeyDeriver {
 public:
  virtual bool FromEKey(Vec<u8>& out, const Str ekey_b64) const = 0;
  virtual bool FromEKey(Vec<u8>& out, const Vec<u8> ekey) const = 0;
  virtual bool ToEKey(Vec<u8>& out, const Vec<u8> key) const = 0;

  static std::unique_ptr<QMCKeyDeriver> Create(
      u8 seed,
      QMCEncV2Stage1Key enc_v2_stage1_key,
      QMCEncV2Stage2Key enc_v2_stage2_key);
};

}  // namespace umc::misc::tencent
