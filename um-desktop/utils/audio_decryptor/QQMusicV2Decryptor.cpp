#include "QQMusicV2Decryptor.h"

#include <um-crypto/qmcv2.h>

using namespace umc;

namespace umd::utils::audio_decryptor {

const usize kInitialDetectionSize = 100;

inline bool QQMusicV2Decryptor::SetupXorCipher(Str& ekey_b64) {
  auto deriver = std::make_unique<qmcv2::SimpleEKeyDerive>();
  Vec<u8> file_key;
  if (!deriver->FromEKey(file_key, ekey_b64)) {
    error_msg_ = _("could not derive file key from ekey");
    return false;
  }

  const auto key_size = file_key.size();
  if (key_size == 0) {
    error_msg_ = _("key size == 0");
    return false;
  }

  if (key_size < 300) {
    cipher_ = std::make_shared<qmcv2::MapCipher>(file_key);
  } else {
    cipher_ = std::make_shared<qmcv2::RC4Cipher>(file_key);
  }

  return true;
}

bool QQMusicV2Decryptor::SetupDecryptor() {
  if (file_size_ < kInitialDetectionSize) {
    return false;
  }

  Vec<u8> fileEOF(kInitialDetectionSize);

  qmcv2::QMCParseError error;
  qmcv2::QMCParsedData qmc_parse_result;
  qmc_parse_result.bytes_required = kInitialDetectionSize;

  do {
    const usize eof_size = qmc_parse_result.bytes_required;
    fileEOF.resize(eof_size);
    in_file_.seekg(-eof_size, std::ifstream::end);
    in_file_.read(reinterpret_cast<char*>(&fileEOF[0]), eof_size);
    error = qmcv2::QMCFileParser::ParseFile(qmc_parse_result, fileEOF);
    if (error == qmcv2::QMCParseError::kOk) {
      bof_bytes_ignore_ = 0;
      eof_bytes_ignore_ = qmc_parse_result.eof_bytes_ignore;

      return SetupXorCipher(qmc_parse_result.ekey_b64);
    }
  } while (error == qmcv2::QMCParseError::kMoreBytesRequired);

  return false;
}

}  // namespace umd::utils::audio_decryptor
