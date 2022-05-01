#pragma once

#include "um-crypto/common.h"

namespace umc::qmcv2 {

/**
 * @brief Derive file key from ekey.
 */
class EKeyDeriveBase {
 public:
  EKeyDeriveBase(){};
  virtual ~EKeyDeriveBase(){};

  bool FromEKey(Vec<u8>& out, const Str ekey_b64) const;
  bool FromEKey(Vec<u8>& out, const Vec<u8> ekey) const;

 protected:
  /**
   * @brief Produce `out.size()` bytes of data.
   *
   * @param out
   */
  virtual void MakeSimpleKey(Vec<u8>& out) const = 0;

 private:
  Vec<u8> DeriveTEAKey(const Vec<u8> ekey) const;
};

class SimpleEKeyDerive : public EKeyDeriveBase {
 public:
  SimpleEKeyDerive(u8 seed = 106) : EKeyDeriveBase() { this->seed = seed; };

 protected:
  void MakeSimpleKey(Vec<u8>& out) const override;

 private:
  u8 seed;
};

}  // namespace umc::qmcv2
