#pragma once

#include "AudioMetadata.h"
#include "AudioTypes.h"

#include "um-crypto/types.h"

namespace umc::utils {

constexpr size_t kAudioTypeSniffBufferSize = 4096;
AudioType DetectAudioType(const u8* buf, usize len);

inline Str DetectAudioExtension(const u8* buf, usize len) {
  return GetAudioTypeExtension(DetectAudioType(buf, len));
}

inline U8Str DetectAudioExtensionU8(const u8* buf, usize len) {
  return U8StrFromStr(DetectAudioExtension(buf, len));
}

}  // namespace umc::utils
