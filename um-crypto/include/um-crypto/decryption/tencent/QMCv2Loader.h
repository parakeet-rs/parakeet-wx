#pragma once

#include "um-crypto/decryption/DecryptionStream.h"
#include "um-crypto/misc/QMCFooterParser.h"
#include "um-crypto/misc/QMCKeyDeriver.h"

namespace umc::decryption::tencent {

class QMCv2Loader : public DecryptionStream {
 public:
  virtual const std::string GetName() const override { return "QMCv2(RC4)"; };

  static std::unique_ptr<QMCv2Loader> Create(
      std::shared_ptr<misc::tencent::QMCFooterParser> parser);
};

}  // namespace umc::decryption::tencent
