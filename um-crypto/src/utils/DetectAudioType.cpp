// References:
// - General sniff code:
//   https://github.com/unlock-music/cli/blob/master/algo/common/sniff.go
// - Various magic numbers:
//   https://www.garykessler.net/library/file_sigs.html
// - DFF:
//   https://www.sonicstudio.com/pdf/dsd/DSDIFF_1.5_Spec.pdf
// - MP3:
//   http://www.mp3-tech.org/programmer/frame_header.html
// - AAC:
//   https://wiki.multimedia.cx/index.php/ADTS
// - FLAC:
//   https://xiph.org/flac/format.html
// - fytp:
//   https://www.ftyps.com/

#include "um-crypto/utils/DetectAudioType.h"
#include "um-crypto/utils/AudioMetadata.h"

#include "um-crypto/endian.h"
#include "um-crypto/types.h"

namespace umc::utils {

inline bool is_mp3(u32 magic) {
  // Framesync, should have first 11-bits set to 1.
  constexpr u32 kMP3AndMasks = 0b1111'1111'1110'0000u << 16;
  constexpr u32 kMP3Expected = 0b1111'1111'1110'0000u << 16;
  return ((magic & kMP3AndMasks) == kMP3Expected);
}

inline bool is_aac(u32 magic) {
  // Framesync, should have first 12-bits set to 1.
  constexpr u32 kAacAndMasks = 0b1111'1111'1111'0110 << 16;
  constexpr u32 kAacExpected = 0b1111'1111'1111'0000 << 16;

  return ((magic & kAacAndMasks) == kAacExpected);
}

constexpr u32 kMagic_fLaC = 0x66'4c'61'43u;  // Free Lossless Audio Codec (FLAC)
constexpr u32 kMagic_OggS = 0x4F'67'67'53u;  // Ogg
constexpr u32 kMagic_FRM8 = 0x46'52'4D'38u;  // Direct Stream Digital (DSDIFF)
constexpr u32 kMagic_ftyp = 0x66'74'79'70u;  // MP4 Frame
constexpr u32 kMagic__wma = 0x30'26'B2'75u;  // Windows WMA/WMV/ASF
constexpr u32 kMagic_RIFF = 0x52'49'46'46u;  // Waveform Audio File Format (WAV)
constexpr u32 kMagic__MAC = 0x4D'41'43'20u;  // Monkey's Audio (APE; u8 "MAC ")

constexpr u32 kMagic_ftyp_MSNV = 0x4d'53'4e'56u;  // MPEG-4 (.MP4) for SonyPSP
constexpr u32 kMagic_ftyp_NDAS = 0x4e'44'41'53u;  // Nero Digital AAC Audio

constexpr u32 kMagic_ftyp_M4A = 0x4d'34'41u;  // iTunes AAC-LC (.M4A) Audio
constexpr u32 kMagic_ftyp_M4B = 0x4d'34'42u;  // iTunes AAC-LC (.M4B) Audio Book

AudioType DetectAudioType(const u8* buf, usize len) {
  // Seek optional id3 tag.
  usize audio_header_meta_size = GetAudioHeaderMetadataSize(buf, len);
  if (audio_header_meta_size > len) {
    return AudioType::kUnknownType;
  } else if (audio_header_meta_size > 0) {
    buf += audio_header_meta_size;
    len -= audio_header_meta_size;
  }

  if (len < 16) {
    return AudioType::kUnknownType;
  }

  {
    // Magic: first 4 bytes
    u32 magic = ReadBigEndian<u32>(buf);

    // 4 byte magics
    switch (magic) {
      case kMagic_fLaC:
        return AudioType::kAudioTypeFLAC;
      case kMagic_OggS:
        return AudioType::kAudioTypeOGG;
      case kMagic_FRM8:
        return AudioType::kAudioTypeDFF;
      case kMagic__wma:
        return AudioType::kAudioTypeWMA;
      case kMagic_RIFF:
        return AudioType::kAudioTypeWAV;
      case kMagic__MAC:
        return AudioType::kAudioTypeAPE;
    }

    // Detect type by its frame header
    if (is_aac(magic)) {
      return AudioType::kAudioTypeAAC;
    } else if (is_mp3(magic)) {
      return AudioType::kAudioTypeMP3;
    }
  }

  // ftyp
  if (ReadBigEndian<u32>(buf + 4) == kMagic_ftyp) {
    u32 magic = ReadBigEndian<u32>(buf + 8);

    if (magic == kMagic_ftyp_MSNV || magic == kMagic_ftyp_NDAS) {
      return AudioType::kAudioTypeM4A;
    }

    switch (magic >> 8) {
      case kMagic_ftyp_M4A:
        return AudioType::kAudioTypeM4A;
      case kMagic_ftyp_M4B:
        return AudioType::kAudioTypeM4B;
    }
  }

  return AudioType::kUnknownType;
}

}  // namespace umc::utils
