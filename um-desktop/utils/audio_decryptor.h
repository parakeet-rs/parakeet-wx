#pragma once

#include <boost/nowide/fstream.hpp>
#include <memory>
#include <string>

#include <um-crypto/abstract/XorStreamCipherBase.h>
#include <um-crypto/qmcv2/file_parser.h>

enum class EncryptionType {
  kNotEncrypted = 0,
  kUnsupported,
  kQQMusic,
  kKugouMusic,
};

class AudioDecryptor {
 public:
  // utf-8 file path.
  AudioDecryptor(const std::string& file_path);
  virtual ~AudioDecryptor(){};

  EncryptionType SniffEncryption();
  bool DecryptAudioFile();

 private:
  boost::nowide::ifstream file_;
  std::string file_path_;
  EncryptionType encryption_ = EncryptionType::kUnsupported;

  std::shared_ptr<umc::qmcv2::QMCParsedData> qmc_parse_result_;

  bool XorDecryptFullFile(std::shared_ptr<umc::XorStreamCipherBase> cipher);
  bool DecryptQQMusicAudio();
};
