#include "um-crypto/kugou/file_parser.h"
#include "kgm_data.h"
#include "um-crypto/endian.h"

#include <cstring>

using namespace umc;
using namespace umc::kugou;

const usize kMinimumKGMHeaderSize = 0x2c;

KGMParseError KGMFileParser::ParseFile(KGMParseResult& result,
                                       const Vec<u8>& header) {
  const usize header_len = header.size();

  if (header_len < kMinimumKGMHeaderSize) {
    result.bytes_required = kMinimumKGMHeaderSize;
    return KGMParseError::kNeedMoreData;
  }

  result.bof_bytes_ignore = ReadLEU32(&header[0x10]);
  result.file_key = Vec<u8>(&header[0x1C], &header[0x2C]);

  if (memcmp(file_header_kgm, header.data(), header_detect_size) == 0) {
    return KGMParseError::kSupportedKGM;
  }

  if (memcmp(file_header_vpr, header.data(), header_detect_size) == 0) {
    return KGMParseError::kSupportedVPR;
  }

  memset(&result, 0, sizeof(result));
  return KGMParseError::kUnsupportedFormat;
}
