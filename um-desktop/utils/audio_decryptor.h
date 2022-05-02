#pragma once

#include <boost/nowide/fstream.hpp>
#include <memory>
#include <string>

#include <um-crypto/abstract/XorStreamCipherBase.h>
#include <um-crypto/common.h>
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
  AudioDecryptor(){};
  virtual ~AudioDecryptor(){};

  void Open(const std::string& file_path);
  EncryptionType SniffEncryption();
  bool DecryptAudioFile();

 private:
  boost::nowide::ifstream file_;
  std::string file_path_;
  std::string error_msg_;
  EncryptionType encryption_ = EncryptionType::kUnsupported;

  umc::usize file_size_ = 0;
  umc::usize bof_skip_count_ = 0;
  umc::usize eof_skip_count_ = 0;

  std::shared_ptr<umc::qmcv2::QMCParsedData> qmc_parse_result_;

  bool XorDecryptFullFile(std::shared_ptr<umc::XorStreamCipherBase> cipher);
  bool DecryptQQMusicAudio();
};
