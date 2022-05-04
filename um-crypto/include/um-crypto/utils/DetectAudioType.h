#pragma once

#include "AudioMetadata.h"
#include "AudioTypes.h"

#include "um-crypto/types.h"

namespace umc::utils {

constexpr size_t kAudioTypeSniffBufferSize = 4096;
AudioType DetectAudioType(const u8* buf, usize len);

inline AudioType DetectAudioType(const Vec<u8>& vec) {
  return DetectAudioType(vec.data(), vec.size());
}

inline bool IsAudioBufferRecognised(const u8* buf, usize len) {
  return DetectAudioType(buf, len) != AudioType::kUnknownType;
}
inline bool IsAudioBufferRecognised(const Vec<u8>& vec) {
  return IsAudioBufferRecognised(vec.data(), vec.size());
}

inline Str DetectAudioExtension(const u8* buf, usize len) {
  return GetAudioTypeExtension(DetectAudioType(buf, len));
}

inline U8Str DetectAudioExtensionU8(const u8* buf, usize len) {
  return U8StrFromStr(DetectAudioExtension(buf, len));
}

}  // namespace umc::utils
