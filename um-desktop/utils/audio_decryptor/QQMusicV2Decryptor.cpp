#include "QQMusicV2Decryptor.h"

#include <um-crypto/tencent.h>

using namespace umc;

namespace umd::utils::audio_decryptor {

constexpr usize kInitialDetectionSize = 100;

inline bool QQMusicV2Decryptor::SetupXorCipher(Str& ekey_b64) {
  auto deriver = std::make_unique<tencent::SimpleEKeyDerive>();
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
    cipher_ = std::make_unique<tencent::MapCipher>(file_key);
  } else {
    cipher_ = std::make_unique<tencent::RC4Cipher>(file_key);
  }

  return true;
}

bool QQMusicV2Decryptor::SetupDecryptor() {
  if (file_size_ < kInitialDetectionSize) {
    return false;
  }

  Vec<u8> fileEOF(kInitialDetectionSize);

  tencent::QMCParseError error;
  tencent::QMCParsedData qmc_parse_result;
  qmc_parse_result.bytes_required = kInitialDetectionSize;

  do {
    const usize eof_size = qmc_parse_result.bytes_required;
    fileEOF.resize(eof_size);
    in_file_.seekg(-eof_size, std::ifstream::end);
    in_file_.read(reinterpret_cast<char*>(&fileEOF[0]), eof_size);
    error = tencent::QMCFileParser::ParseFile(qmc_parse_result, fileEOF);
    if (error == tencent::QMCParseError::kOk) {
      bof_bytes_ignore_ = 0;
      eof_bytes_ignore_ = qmc_parse_result.eof_bytes_ignore;

      return SetupXorCipher(qmc_parse_result.ekey_b64);
    }
  } while (error == tencent::QMCParseError::kMoreBytesRequired);

  return false;
}

}  // namespace umd::utils::audio_decryptor
