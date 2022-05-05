#include "KuwoMusicDecryptor.h"

#include <um-crypto/kuwo.h>

using namespace umc;
using namespace umc::kuwo;

namespace umd::utils::audio_decryptor {

bool KuwoMusicDecryptor::SetupDecryptor() {
  if (file_size_ < kKuwoDetectionSize) {
    return false;
  }

  u8 file_header[kKuwoDetectionSize];
  if (Read(file_header, kKuwoDetectionSize) != kKuwoDetectionSize) {
    return false;
  }

  auto config = KWMFileParser::ParseKWMFile(file_header, kKuwoDetectionSize);
  if (!config) {
    return false;
  }

  cipher_ = std::make_unique<KWMCipher>(config->file_key);
  bof_bytes_ignore_ = kKuwoHeaderSkipSize;
  eof_bytes_ignore_ = 0;

  return true;
}

}  // namespace umd::utils::audio_decryptor
