#pragma once

#include "um-crypto/decryption/DecryptionStream.h"
#include "um-crypto/misc/QMCFooterParser.h"
#include "um-crypto/misc/QMCKeyDeriver.h"

namespace umc::decryption::tencent {

typedef Vec<u8> QMCv1Key;

class QMCv1Loader : public DecryptionStream {
 public:
  virtual const Str GetName() const override { return "QMCv1(static/map)"; };

  static std::unique_ptr<QMCv1Loader> Create(const QMCv1Key& key);
  static std::unique_ptr<QMCv1Loader> Create(
      std::shared_ptr<misc::tencent::QMCFooterParser> parser);
};

}  // namespace umc::decryption::tencent
