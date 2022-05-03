#pragma once

#include "um-crypto/common.h"
#include "um-crypto/kugou/constants.h"

namespace umc::kugou {

enum class KGMParseError {
  kUnsupportedFormat = 0,
  kNeedMoreData,

  kSupportedKGM,
  kSupportedVPR,
};

struct KGMParseResult {
  // numbe of bytes to ignore at the beginning of file.
  u32 bof_bytes_ignore;
  KugouFileKey file_key;

  // Error report
  usize bytes_required;
};

class KGMFileParser {
 public:
  static KGMParseError ParseFile(KGMParseResult& result, const Vec<u8>& header);
};

}  // namespace umc::kugou
