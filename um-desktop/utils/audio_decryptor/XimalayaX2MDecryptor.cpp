#include "XimalayaX2MDecryptor.h"

#include <um-crypto/ximalaya.h>

using namespace umc::ximalaya;
namespace umd::utils::audio_decryptor {

bool XimalayaX2MDecryptor::SetupDecryptor() {
  X2MHeader header;
  in_file_.seekg(0, std::ifstream::beg);
  if (kX2MEncryptedHeaderSize != Read(header.data(), kX2MEncryptedHeaderSize)) {
    return false;
  }

  if (!XimalayaFileParser::IsX2MFile(header)) {
    return false;
  }

  bof_bytes_ignore_ = 0;
  eof_bytes_ignore_ = 0;
  cipher_ = std::make_shared<X2MCipher>(header);
  return true;
}

}  // namespace umd::utils::audio_decryptor
