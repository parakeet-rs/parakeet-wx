#include "um-crypto/kugou/KGMFileParser.h"

#include "um-crypto/endian.h"
#include "um-crypto/kugou/constants.h"

#include <cstring>

using namespace umc;
using namespace umc::kugou;

constexpr usize kMinimumKGMHeaderSize = 0x2c;

KGMParseError KGMFileParser::ParseFile(KGMParseResult& result,
                                       const Vec<u8>& header) {
  const usize header_len = header.size();

  if (header_len < kMinimumKGMHeaderSize) {
    result.bytes_required = kMinimumKGMHeaderSize;
    return KGMParseError::kNeedMoreData;
  }

  result.bof_bytes_ignore = ReadLittleEndian<u32>(&header[0x10]);
  memcpy(result.file_key.data(), &header.at(0x1c), kFileKeySize);
  result.file_key[16] = 0;  // last byte is always a "\x00";

  if (memcmp(kFileHeaderKGM.data(), header.data(), kFormatHeaderSize) == 0) {
    return KGMParseError::kSupportedKGM;
  }

  if (memcmp(kFileHeaderVPR.data(), header.data(), kFormatHeaderSize) == 0) {
    return KGMParseError::kSupportedVPR;
  }

  memset(&result, 0, sizeof(result));
  return KGMParseError::kUnsupportedFormat;
}
