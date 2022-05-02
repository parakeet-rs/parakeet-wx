#pragma once
#include "AStreamDecryptor.h"

namespace umd::utils::audio_decryptor {

class KugouMusicDecryptor : public AStreamDecryptor {
  EncryptionType GetEncryptionType() const override {
    return EncryptionType::kKugouMusic;
  };

  virtual bool SetupDecryptor() override;

 private:
};

}  // namespace umd::utils::audio_decryptor
