#pragma once
#include "AStreamDecryptor.h"

namespace umd::utils::audio_decryptor {

class QQMusicV2Decryptor : public AStreamDecryptor {
 public:
  EncryptionType GetEncryptionType() const override {
    return EncryptionType::kQQMusicV2;
  };

  virtual bool SetupDecryptor() override;

 private:
  Str ekey_b64;
  bool SetupXorCipher(Str& ekey_b64);
};

}  // namespace umd::utils::audio_decryptor
