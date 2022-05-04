#pragma once
#include "AStreamDecryptor.h"

namespace umd::utils::audio_decryptor {

class XimalayaX2MDecryptor : public AStreamDecryptor {
 public:
  EncryptionType GetEncryptionType() const override {
    return EncryptionType::kXimalayaX2M;
  };

  virtual bool SetupDecryptor() override;

 private:
};

}  // namespace umd::utils::audio_decryptor
