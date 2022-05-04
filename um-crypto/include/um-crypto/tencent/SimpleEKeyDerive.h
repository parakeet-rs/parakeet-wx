#pragma once

#include "um-crypto/types.h"

namespace umc::tencent {

/**
 * @brief Derive file key from ekey.
 */
class AEKeyDerive {
 public:
  AEKeyDerive(){};
  virtual ~AEKeyDerive(){};

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

class SimpleEKeyDerive : public AEKeyDerive {
 public:
  SimpleEKeyDerive(u8 seed = 106) : AEKeyDerive() { this->seed = seed; };

 protected:
  void MakeSimpleKey(Vec<u8>& out) const override;

 private:
  u8 seed;
};

}  // namespace umc::tencent
