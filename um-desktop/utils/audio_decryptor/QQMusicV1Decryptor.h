#pragma once
#include "AStreamDecryptor.h"

namespace umd::utils::audio_decryptor {

class QQMusicV1Decryptor : public AStreamDecryptor {
 public:
  EncryptionType GetEncryptionType() const override {
    return EncryptionType::kQQMusicV1;
  };

  virtual bool SetupDecryptor() override;
};

}  // namespace umd::utils::audio_decryptor
