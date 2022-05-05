#pragma once
#include "common.h"

#include "um-crypto/types.h"

#include <utility>

namespace umc::ximalaya {

class XimalayaFileParser {
 public:
  static bool IsX2MFile(const XimalayaAndroidFileHeader& header);
  static bool IsX3MFile(const XimalayaAndroidFileHeader& header);
};

}  // namespace umc::ximalaya
