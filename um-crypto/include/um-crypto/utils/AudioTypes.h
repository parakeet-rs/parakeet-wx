#pragma once

#include "um-crypto/types.h"

namespace umc::utils {
constexpr u32 kAudioTypeMaskLossless = 1 << 5;

enum class AudioType : u32 {
  kUnknownType = 0,

  // Lossy
  kAudioTypeOGG,
  kAudioTypeAAC,
  kAudioTypeMP3,
  kAudioTypeM4A,
  kAudioTypeM4B,
  kAudioTypeMP4,

  // Lossless
  kAudioTypeFLAC = kAudioTypeMaskLossless | 0,
  kAudioTypeDFF,
  kAudioTypeWAV,
  kAudioTypeWMA,
  kAudioTypeAPE,
};

inline std::string GetAudioTypeExtension(AudioType type) {
  switch (type) {
    case AudioType::kAudioTypeOGG:
      return "ogg";
    case AudioType::kAudioTypeAAC:
      return "aac";
    case AudioType::kAudioTypeMP3:
      return "mp3";
    case AudioType::kAudioTypeM4A:
      return "m4a";
    case AudioType::kAudioTypeM4B:
      return "m4b";
    case AudioType::kAudioTypeMP4:
      return "mp4";
    case AudioType::kAudioTypeFLAC:
      return "flac";
    case AudioType::kAudioTypeDFF:
      return "dff";
    case AudioType::kAudioTypeWAV:
      return "wav";
    case AudioType::kAudioTypeWMA:
      return "wma";
    case AudioType::kAudioTypeAPE:
      return "ape";
  }

  return "bin";
}

inline bool AudioIsLossless(AudioType type) {
  return (static_cast<u32>(type) & kAudioTypeMaskLossless) != 0;
}

}  // namespace umc::utils
