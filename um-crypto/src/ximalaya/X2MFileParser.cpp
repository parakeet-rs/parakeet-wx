#include "um-crypto/ximalaya/X2MFileParser.h"

#include "um-crypto/endian.h"
#include "um-crypto/ximalaya/X2MCipher.h"

#include <memory>

namespace umc::ximalaya {

const u32 kMagic_ftyp = 0x66'74'79'70u;  // m4a header

bool XimalayaFileParser::IsX2MFile(const X2MHeader& header) {
  u8 buf[4];
  usize len = sizeof(buf) / sizeof(buf[0]);

  auto decryptor = std::make_unique<X2MCipher>(header);
  decryptor->Seek(4);
  decryptor->Decrypt(buf, len, header.data(), len);

  return ReadBEU32(buf) == kMagic_ftyp;
}

};  // namespace umc::ximalaya
