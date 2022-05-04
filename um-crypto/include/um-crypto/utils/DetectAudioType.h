#pragma once

#include "AudioMetadata.h"
#include "AudioTypes.h"

#include "um-crypto/common.h"

namespace umc::utils {

const size_t kAudioTypeSniffBufferSize = 1024;
AudioType DetectAudioType(const u8* buf, usize len);

inline Str DetectAudioExtension(const u8* buf, usize len) {
  return GetAudioTypeExtension(DetectAudioType(buf, len));
}

inline U8Str DetectAudioExtensionU8(const u8* buf, usize len) {
  return U8StrFromStr(DetectAudioExtension(buf, len));
}

}  // namespace umc::utils
