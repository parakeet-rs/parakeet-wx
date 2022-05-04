#pragma once
#include "common.h"

#include "um-crypto/common.h"

#include <utility>

namespace umc::ximalaya {

class XimalayaFileParser {
 public:
  static bool IsX2MFile(const X2MHeader& header);
};

}  // namespace umc::ximalaya
