#include "um-crypto/kuwo/KWMFileParser.h"
#include "um-crypto/endian.h"

namespace umc::kuwo {
const u8 kKuwoFileHeader[16] = {0x79, 0x65, 0x65, 0x6c, 0x69, 0x6f, 0x6e, 0x2d,
                                0x6b, 0x75, 0x77, 0x6f, 0x2d, 0x74, 0x6d, 0x65};

std::unique_ptr<KWMConfig> KWMFileParser::ParseKWMFile(const u8* header,
                                                       usize len) {
  if (len < kKuwoDetectionSize) {
    return nullptr;
  }
  auto kwm_config = std::make_unique<KWMConfig>();
  kwm_config->file_key = ReadLEU64(&header[0x18]);
  return std::move(kwm_config);
}

}  // namespace umc::kuwo
