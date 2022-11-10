#pragma once

#include "AudioMetadata.h"
#include "AudioTypes.h"

namespace umc::utils {

constexpr size_t kAudioTypeSniffBufferSize = 4096;
AudioType DetectAudioType(const uint8_t* buf, std::size_t len);

inline AudioType DetectAudioType(const std::vector<uint8_t>& vec) {
  return DetectAudioType(vec.data(), vec.size());
}

inline bool IsAudioBufferRecognised(const uint8_t* buf, std::size_t len) {
  return DetectAudioType(buf, len) != AudioType::kUnknownType;
}
inline bool IsAudioBufferRecognised(const std::vector<uint8_t>& vec) {
  return IsAudioBufferRecognised(vec.data(), vec.size());
}

inline std::string DetectAudioExtension(const uint8_t* buf, std::size_t len) {
  return GetAudioTypeExtension(DetectAudioType(buf, len));
}

inline std::u8string DetectAudioExtensionU8(const uint8_t* buf, std::size_t len) {
  auto str = DetectAudioExtension(buf, len);
  return std::u8string(str.begin(), str.end());
}

}  // namespace umc::utils
