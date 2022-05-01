#pragma once

#include "um-crypto/common.h"

namespace umc::kugou {

enum class KGMParseError {
  kUnsupportedFormat = 0,
  kNeedMoreData,

  kSupportedKGM,
  kSupportedVPR,
};

struct KGMParseResult {
  u32 encrypted_offset;
  Vec<u8> file_key;

  // Error report
  usize bytes_required;
};

class KGMFileParser {
  static KGMParseError ParseFile(KGMParseResult& result, const Vec<u8>& header);
};

}  // namespace umc::kugou
