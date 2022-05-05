#include "um-crypto/xiami/XMFileParser.h"

#include "um-crypto/endian.h"

namespace umc::xiami {

// 'ifmt +' \xfe *4
constexpr u32 kXiamiHeaderMagic1 = 0x69'66'6D'74;
constexpr u32 kXiamiHeaderMagic2 = 0xfe'fe'fe'fe;

std::unique_ptr<XiamiXMConfig> XiamiFileParser::ParseXiamiXMFile(
    const XiamiHeader& header) {
  if (ReadBEU32(&header[0x00]) != kXiamiHeaderMagic1 ||
      ReadBEU32(&header[0x08]) != kXiamiHeaderMagic2) {
    return nullptr;
  }

  auto config = std::make_unique<XiamiXMConfig>();
  config->file_key = header[0x0f];

  // Little endian, 24-bit.
  config->noop_len = ReadLEU32(&header[0x0c]) & 0x00ffffff;

  return config;
}

}  // namespace umc::xiami
