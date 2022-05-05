#pragma once
#include "um-crypto/types.h"

#include <utility>

namespace umc::kuwo {

constexpr usize kKuwoDetectionSize = 0x20;

// When decoding KWM file, this size should be skipped.
constexpr usize kKuwoHeaderSkipSize = 0x400;

struct KWMConfig {
  u64 file_key;
};

class KWMFileParser {
 public:
  static std::unique_ptr<KWMConfig> ParseKWMFile(const u8* header, usize len);
};

}  // namespace umc::kuwo
