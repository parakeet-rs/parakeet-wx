#include "../audio_type_sniff.h"
#include "um-crypto/common.h"
#include "um-crypto/endian.h"

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

using namespace umc;
namespace umd::utils {

inline bool is_mp3(u32 magic, size_t len) {
  const u32 kID3Masks = 0xFF'FF'FF'00u;  // Select first 3 bytes
  const u32 kID3Value = 0x49'44'33'00u;  // 'ID3\x00'
  if ((magic & kID3Masks) == kID3Value) {
    return true;
  }

  const u32 kMP3AndMasks = 0b1111'1111'1110'0000u << 16;
  const u32 kMP3Expected = 0b1111'1111'1110'0000u << 16;
  // Framesync, should have 11 bits set to 1.
  return ((magic & kMP3AndMasks) == kMP3Expected);
}

inline bool is_aac(u32 magic, size_t len) {
  const u32 kAacAndMasks = 0b1111'1111'1111'0110 << 16;
  const u32 kAacExpected = 0b1111'1111'1111'0000 << 16;

  return ((magic & kAacAndMasks) == kAacExpected);
}

const u32 kMagic_fLaC = 0x66'4c'61'43u;  // Free Lossless Audio Codec (FLAC)
const u32 kMagic_OggS = 0x4F'67'67'53u;  // Ogg
const u32 kMagic_FRM8 = 0x46'52'4D'38u;  // Direct Stream Digital (DSDIFF)
const u32 kMagic_ftyp = 0x66'74'79'70u;  // MP4 Frame
const u32 kMagic__wma = 0x30'26'B2'75u;  // Windows WMA/WMV/ASF

const u32 kMagic_ftyp_MSNV = 0x4d'53'4e'56u;  // MPEG-4 (.MP4) for SonyPSP
const u32 kMagic_ftyp_NDAS = 0x4e'44'41'53u;  // Nero Digital AAC Audio

const u32 kMagic_ftyp_M4A = 0x4d'34'41u;  // iTunes AAC-LC (.M4A) Audio
const u32 kMagic_ftyp_M4B = 0x4d'34'42u;  // iTunes AAC-LC (.M4B) Audio Book

std::string AudioSniffSimple(const uint8_t* buf, size_t len) {
  if (len < 16) {
    return "";
  }

  {
    // Magic: first 4 bytes
    u32 magic = ReadBEU32(buf);

    // 4 byte magics
    switch (magic) {
      case kMagic_fLaC:
        return "flac";
      case kMagic_OggS:
        return "ogg";
      case kMagic_FRM8:
        return "dff";
      case kMagic__wma:
        return "wma";
    }

    // Compact header magics
    if (is_aac(magic, len)) {
      return "aac";
    }

    if (is_mp3(magic, len)) {
      return "mp3";
    }
  }

  // ftyp
  if (ReadBEU32(buf + 4) == kMagic_ftyp) {
    u32 magic = ReadBEU32(buf + 8);

    if (magic == kMagic_ftyp_MSNV || kMagic_ftyp_NDAS) {
      return "m4a";
    }

    switch (magic >> 8) {
      case kMagic_ftyp_M4A:
        return "m4a";
      case kMagic_ftyp_M4B:
        return "m4b";
    }
  }

  return "bin";
}

}  // namespace umd::utils
