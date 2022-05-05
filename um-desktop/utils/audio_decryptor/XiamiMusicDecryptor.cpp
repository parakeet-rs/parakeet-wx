#include "XiamiMusicDecryptor.h"

#include <um-crypto/xiami.h>

using namespace umc;
using namespace umc::xiami;

namespace umd::utils::audio_decryptor {

bool XiamiMusicDecryptor::SetupDecryptor() {
  if (file_size_ < kXiamiHeaderSize) {
    return false;
  }

  XiamiHeader file_header;
  if (Read(file_header.data(), kXiamiHeaderSize) != kXiamiHeaderSize) {
    return false;
  }

  auto config = XiamiFileParser::ParseXiamiXMFile(file_header);
  if (!config) {
    return false;
  }

  cipher_ = std::make_unique<XMCipher>(*config);
  bof_bytes_ignore_ = kXiamiSkipBOFSize;
  eof_bytes_ignore_ = 0;

  return true;
}

}  // namespace umd::utils::audio_decryptor
