#pragma once
#include "AStreamDecryptor.h"

namespace umd::utils::audio_decryptor {

class XimalayaX3MDecryptor : public AStreamDecryptor {
 public:
  EncryptionType GetEncryptionType() const override {
    return EncryptionType::kXimalayaX3M;
  };

  virtual bool SetupDecryptor() override;

 private:
};

}  // namespace umd::utils::audio_decryptor
