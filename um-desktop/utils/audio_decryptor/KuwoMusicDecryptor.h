#pragma once
#include "AStreamDecryptor.h"

namespace umd::utils::audio_decryptor {

class KuwoMusicDecryptor : public AStreamDecryptor {
 public:
  EncryptionType GetEncryptionType() const override {
    return EncryptionType::kKuwoMusic;
  };

  virtual bool SetupDecryptor() override;

 private:
};

}  // namespace umd::utils::audio_decryptor
