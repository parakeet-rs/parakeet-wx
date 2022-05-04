#pragma once
#include "audio_decryptor/ADecryptor.h"
#include "common.h"

#include <memory>

namespace umd::utils {
using umd::utils::audio_decryptor::ADecryptor;
using umd::utils::audio_decryptor::EncryptionType;

class AudioDecryptorManager {
 public:
  AudioDecryptorManager();
  virtual ~AudioDecryptorManager(){};

  void Open(const Path& file_path);
  void Close();

  EncryptionType SniffEncryption();
  bool DecryptAudioFile();
  const Str& GetError() {
    return active_decryptor_ ? active_decryptor_->GetErrorMessage() : error_;
  }

 private:
  Path in_file_path_;
  Str error_ = "unknown error";

  std::shared_ptr<ADecryptor> active_decryptor_;
  Vec<std::shared_ptr<ADecryptor>> decryptors_;

  /**
   * @brief Helper to add a new decryptor.
   *
   * @param decryptor
   */
  inline void Add(std::shared_ptr<ADecryptor> decryptor) {
    decryptors_.push_back(decryptor);
  }
};

}  // namespace umd::utils
