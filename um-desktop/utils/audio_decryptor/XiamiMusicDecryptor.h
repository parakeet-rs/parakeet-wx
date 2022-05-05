#pragma once
#include "AStreamDecryptor.h"

namespace umd::utils::audio_decryptor {

class XiamiMusicDecryptor : public AStreamDecryptor {
 public:
  EncryptionType GetEncryptionType() const override {
    return EncryptionType::kXiamiMusic;
  };

  virtual bool SetupDecryptor() override;

 private:
};

}  // namespace umd::utils::audio_decryptor
