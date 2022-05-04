#include "QQMusicV1Decryptor.h"

#include <um-crypto/tencent.h>

using namespace umc;
using namespace umc::tencent;

namespace umd::utils::audio_decryptor {

bool QQMusicV1Decryptor::SetupDecryptor() {
  constexpr usize len = kLegacyQMCDetectionSize;
  if (file_size_ < len) {
    return false;
  }

  Vec<u8> file_bof(len);
  in_file_.seekg(0, std::ifstream::beg);
  if (Read(file_bof.data(), len) != len) {
    return false;
  }

  if (!QMCFileParser::IsLegacyQMCFile(file_bof)) {
    return false;
  }

  cipher_ = std::make_unique<StaticStreamCipher>();
  return true;
}

}  // namespace umd::utils::audio_decryptor
