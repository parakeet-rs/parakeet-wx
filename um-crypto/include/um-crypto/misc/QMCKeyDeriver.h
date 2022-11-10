#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <cstdint>

namespace umc::misc::tencent {
typedef std::array<uint8_t, 16> QMCEncV2Stage1Key;
typedef std::array<uint8_t, 16> QMCEncV2Stage2Key;

class QMCKeyDeriver {
 public:
  virtual bool FromEKey(std::vector<uint8_t>& out, const std::string ekey_b64) const = 0;
  virtual bool FromEKey(std::vector<uint8_t>& out, const std::vector<uint8_t> ekey) const = 0;
  virtual bool ToEKey(std::vector<uint8_t>& out, const std::vector<uint8_t> key) const = 0;

  static std::unique_ptr<QMCKeyDeriver> Create(uint8_t seed,
                                               QMCEncV2Stage1Key enc_v2_stage1_key,
                                               QMCEncV2Stage2Key enc_v2_stage2_key);
};

}  // namespace umc::misc::tencent
