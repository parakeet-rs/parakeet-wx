#include "audio_decryptor.h"
#include "audio_type_sniff.h"

#include <um-crypto/qmcv2.h>

#include <boost/filesystem.hpp>

#include <algorithm>
#include <fstream>
#include <vector>

using namespace umc;
using namespace umd;
using namespace umd::utils;

const usize kDecryptBufferSize = 4 * 1024 * 1024;

namespace nowide = boost::nowide;

void AudioDecryptor::Open(const std::string& file_path) {
  file_path_ = file_path;
  file_.open(file_path_, std::ifstream::in | std::ifstream::binary);
}

const size_t kQMCDetectionSize = 100;

EncryptionType AudioDecryptor::SniffEncryption() {
  file_.seekg(0, std::ifstream::end);
  auto file_size = file_size_ = file_.tellg();

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
        bof_skip_count_ = 0;
        eof_skip_count_ = qmc_parse_result_->eof_bytes_ignore;
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
  Vec<u8> buf_encrypted(kDecryptBufferSize);
  Vec<u8> buf_decrypted(kDecryptBufferSize);

  // Input pointer, signed char
  auto p_i8_encrypted = reinterpret_cast<char*>(buf_encrypted.data());
  auto p_i8_decrypted = reinterpret_cast<char*>(buf_decrypted.data());

  // Calculate the number of bytes needs processing
  usize bytes_to_process = file_size_ - bof_skip_count_ - eof_skip_count_;

  // Create mapping between input and output.
  cipher->Seek(0);
  file_.seekg(bof_skip_count_, std::ifstream::beg);

  // Decrypt the first block
  file_.read(p_i8_encrypted, kDecryptBufferSize);
  auto first_read_len = file_.gcount();
  cipher->XorStream(buf_decrypted.data(), buf_encrypted.data(), first_read_len);

  // Detect audio file extension from decrypted buffer.
  // TODO: Erase original extension.
  auto type = SniffAudioType(buf_decrypted.data(), first_read_len);
  auto output_file_path = file_path_ + "." + type;

  boost::nowide::ofstream f_out(output_file_path, std::ofstream::binary);
  f_out.write(p_i8_decrypted, first_read_len);
  bytes_to_process -= first_read_len;

  while (bytes_to_process > 0) {
    const usize block_size = std::min(bytes_to_process, kDecryptBufferSize);

    // Read data
    buf_encrypted.resize(block_size);
    file_.read(p_i8_encrypted, block_size);

    // Get the number of bytes read.
    const usize bytes_read = file_.gcount();
    if (bytes_read == 0) {
      error_msg_ = _("Error: Unexpected EOF when reading input.");
      break;
    }

    // Transform bytes read.
    cipher->XorStream(buf_decrypted.data(), buf_encrypted.data(), bytes_read);
    f_out.write(p_i8_decrypted, bytes_read);
    bytes_to_process -= block_size;
  };

  file_.close();
  f_out.close();
  return bytes_to_process == 0;
}
