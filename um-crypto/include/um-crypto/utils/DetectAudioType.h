#pragma once

#include "AudioMetadata.h"
#include "AudioTypes.h"

#include "um-crypto/types.h"

namespace umc::utils {

constexpr size_t kAudioTypeSniffBufferSize = 4096;
AudioType DetectAudioType(const u8* buf, std::size_t len);

inline AudioType DetectAudioType(const std::vector<u8>& vec) {
  return DetectAudioType(vec.data(), vec.size());
}

inline bool IsAudioBufferRecognised(const u8* buf, std::size_t len) {
  return DetectAudioType(buf, len) != AudioType::kUnknownType;
}
inline bool IsAudioBufferRecognised(const std::vector<u8>& vec) {
  return IsAudioBufferRecognised(vec.data(), vec.size());
}

inline std::string DetectAudioExtension(const u8* buf, std::size_t len) {
  return GetAudioTypeExtension(DetectAudioType(buf, len));
}

inline std::u8string DetectAudioExtensionU8(const u8* buf, std::size_t len) {
  return U8StrFromStr(DetectAudioExtension(buf, len));
}

}  // namespace umc::utils
