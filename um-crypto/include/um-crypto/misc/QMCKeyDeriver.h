#pragma once

#include "um-crypto/types.h"

namespace umc::misc::tencent {
typedef std::array<u8, 16> QMCEncV2Stage1Key;
typedef std::array<u8, 16> QMCEncV2Stage2Key;

class QMCKeyDeriver {
 public:
  virtual bool FromEKey(std::vector<u8>& out, const std::string ekey_b64) const = 0;
  virtual bool FromEKey(std::vector<u8>& out, const std::vector<u8> ekey) const = 0;
  virtual bool ToEKey(std::vector<u8>& out, const std::vector<u8> key) const = 0;

  static std::unique_ptr<QMCKeyDeriver> Create(u8 seed,
                                               QMCEncV2Stage1Key enc_v2_stage1_key,
                                               QMCEncV2Stage2Key enc_v2_stage2_key);
};

}  // namespace umc::misc::tencent
