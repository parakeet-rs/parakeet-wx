#pragma once

#include "../DecryptionStream.h"

namespace umc::decryption::tencent {

typedef Vec<u8> QMCv1Key;

class QMCv1Loader : public DecryptionStream {
 public:
  virtual const Str GetName() const override { return "QMCv1(static/map)"; };

  static std::unique_ptr<QMCv1Loader> CreateStatic(const QMCv1Key& key);
  static std::unique_ptr<QMCv1Loader> CreateMap(const QMCv1Key& key);
};

}  // namespace umc::decryption::tencent
