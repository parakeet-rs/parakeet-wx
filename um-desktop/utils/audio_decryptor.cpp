#include "audio_decryptor.h"
#include "audio_type_sniff.h"

#include <um-crypto/qmcv2.h>

#include <boost/filesystem.hpp>
#include <fstream>
#include <vector>

using namespace umc;

const usize kDecryptBufferSize = 4 * 1024 * 1024;

namespace nowide = boost::nowide;

AudioDecryptor::AudioDecryptor(const std::string& file_path) {
  file_path_ = file_path;
  file_.open(file_path_, std::ifstream::in | std::ifstream::binary);
}

const size_t kQMCDetectionSize = 100;

EncryptionType AudioDecryptor::SniffEncryption() {
  file_.seekg(0, std::ifstream::end);
  auto file_size = file_.tellg();

  // Sniff for QMCv2
  if (file_size > kQMCDetectionSize) {
    Vec<u8> fileEOF(kQMCDetectionSize);
    qmc_parse_result_ = std::make_shared<qmcv2::QMCParsedData>();
    qmcv2::QMCParseError err;

    qmc_parse_result_->bytes_required = kQMCDetectionSize;

    do {
      const usize eof_size = qmc_parse_result_->bytes_required;
      fileEOF.resize(eof_size);
      file_.seekg(-eof_size, std::ifstream::end);
      file_.read(reinterpret_cast<char*>(&fileEOF[0]), eof_size);
      err = qmcv2::QMCFileParser::ParseFile(*qmc_parse_result_, fileEOF);
      if (err == qmcv2::QMCParseError::kOk) {
        encryption_ = EncryptionType::kQQMusic;
        return EncryptionType::kQQMusic;
      }
    } while (err == qmcv2::QMCParseError::kMoreBytesRequired);
  }

  return EncryptionType::kUnsupported;
}

bool AudioDecryptor::DecryptAudioFile() {
  if (encryption_ == EncryptionType::kQQMusic) {
    return DecryptQQMusicAudio();
  }

  return false;
}

bool AudioDecryptor::DecryptQQMusicAudio() {
  Vec<u8> key;

  qmcv2::SimpleEKeyDerive* deriver = new qmcv2::SimpleEKeyDerive();
  deriver->FromEKey(key, qmc_parse_result_->ekey_b64);
  delete deriver;

  if (key.size() == 0) {
    return false;
  }

  std::shared_ptr<XorStreamCipherBase> stream_decryptor;
  if (key.size() > 300) {
    stream_decryptor = std::make_shared<qmcv2::RC4Cipher>(key);
  } else {
    stream_decryptor = std::make_shared<qmcv2::MapCipher>(key);
  }

  return XorDecryptFullFile(stream_decryptor);
}

bool AudioDecryptor::XorDecryptFullFile(
    std::shared_ptr<XorStreamCipherBase> cipher) {
  Vec<u8> buf_input(4096);
  Vec<u8> buf_output(4096);

  cipher->Seek(0);
  file_.seekg(0, std::ifstream::beg);
  file_.read(reinterpret_cast<char*>(&buf_input[0]), 4096);
  buf_input.resize(file_.gcount());
  cipher->XorStream(buf_output, buf_input);

  auto type = SniffAudioType(buf_output.data(), buf_output.size());

  // TODO: Check for the value returned here.

  auto output_file_path = file_path_ + "." + type;
  boost::nowide::ofstream decrypted_file(output_file_path,
                                         std::ofstream::binary);

  decrypted_file.write(reinterpret_cast<char*>(buf_output.data()),
                       buf_output.size());

  do {
    buf_input.resize(kDecryptBufferSize);
    file_.read(reinterpret_cast<char*>(&buf_input[0]), kDecryptBufferSize);
    const auto bytes_read = file_.gcount();
    buf_input.resize(bytes_read);
    if (bytes_read == 0)
      break;

    cipher->XorStream(buf_output, buf_input);
    decrypted_file.write(reinterpret_cast<char*>(buf_output.data()),
                         buf_output.size());
  } while (true);
  decrypted_file.close();

  bool is_eof = file_.eof();
  file_.close();
  return is_eof;
}
