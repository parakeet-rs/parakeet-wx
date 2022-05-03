#pragma once
#include "audio_decryptor/ADecryptor.h"

#include <memory>

namespace umd::utils {
using umd::utils::audio_decryptor::ADecryptor;
using umd::utils::audio_decryptor::EncryptionType;

class AudioDecryptorManager {
 public:
  AudioDecryptorManager();
  virtual ~AudioDecryptorManager(){};

  void Open(const std::string& file_path);
  void Close();

  EncryptionType SniffEncryption();
  bool DecryptAudioFile();
  const std::string& GetError() {
    return active_decryptor_ ? active_decryptor_->GetErrorMessage() : error_;
  }

 private:
  std::string in_file_path_;
  std::string error_ = "unknown error";

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
