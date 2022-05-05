#pragma once
#include "common.h"

namespace umc::xiami {

class XiamiFileParser {
 public:
  static std::unique_ptr<XiamiXMConfig> ParseXiamiXMFile(
      const XiamiHeader& header);
};

}  // namespace umc::xiami
