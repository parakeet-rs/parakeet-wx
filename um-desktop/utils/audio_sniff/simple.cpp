#include "simple.h"

using namespace umd::utils;

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

inline bool is_mp3(const uint8_t* buf, size_t len) {
  if (memcmp(buf, "ID3", 3) == 0) {
    return true;
  }

  // Framesync, should have 11 bits set to 1.
  if (buf[0] == 0xFF && (buf[0] & 0x0b11100000) == 0x0b11100000) {
    return true;
  }

  return false;
}

inline bool is_aac(const uint8_t* buf, size_t len) {
  // https://wiki.multimedia.cx/index.php/ADTS
  if (buf[0] == 0xFF && (buf[0] & 0b11110110) == 0b11110000) {
    return true;
  }

  return false;
}

inline bool Compare4Bytes(const uint8_t* a, const uint8_t* b) {
  return (a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]);
}

inline bool CompareBytes(const uint8_t* a, const uint8_t* b, int n) {
  for (int i = 0; i < n; i++) {
    if (a[i] != b[i])
      return false;
  }
  return true;
}

std::string audio_sniff_simple(const uint8_t* buf, size_t len) {
  if (len < 4) {
    return "";
  }

  if (is_aac(buf, len) == 0) {
    return "aac";
  }

  if (is_mp3(buf, len) == 0) {
    return "mp3";
  }

  if (Compare4Bytes(buf, "fLaC")) {
    return "flac";
  }

  if (Compare4Bytes(buf, "OggS")) {
    return "ogg";
  }

  // ckID is always 'FRM8'.
  if (Compare4Bytes(buf, "FRM8")) {
    return "dff";
  }

  // https://www.ftyps.com/
  if (Compare4Bytes(&buf[4], "ftyp")) {
    auto p_type = &buf[8];
#define CHECK_FILE_TYPE(SEQUENCE, EXTENSION)                  \
  if (CompareBytes(p_type, SEQUENCE, sizeof(SEQUENCE) - 1)) { \
    return EXTENSION;                                         \
  }

    // 3gpp
    CHECK_FILE_TYPE("3g2", "3g2");
    CHECK_FILE_TYPE("3ge", "3gp");
    CHECK_FILE_TYPE("3gg", "3gp");
    CHECK_FILE_TYPE("3gp", "3gp");

    CHECK_FILE_TYPE("avc", "avc");  // MP4 Base w/ AVC ext [ISO 14496-12:2005]
  }

  // TODO: implement more checks

  return "bin";
}
