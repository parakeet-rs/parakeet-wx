#pragma once

#include "um-crypto/endian.h"
#include "um-crypto/types.h"

namespace umc::utils {

inline i32 ParseID3SyncSafeInt(const u8* p) {
  auto raw = umc::ReadBigEndian<u32>(p);

  // Sync safe int should use only lower 7-bits of each byte.
  if ((raw & 0x80808080u) != 0) {
    return 0;
  }

  return ((raw & 0x7F00'0000) >> 3) | ((raw & 0x007F'0000) >> 2) |
         ((raw & 0x0000'7F00) >> 1) | ((raw & 0x0000'007F) >> 0);
}

inline std::size_t GetID3HeaderSize(u32 magic, const u8* buf, std::size_t len) {
  if (len < 10) {
    return 0;
  }

  // ID3v1 and ID3v1.1: flat 128 bytes
  constexpr u32 kID3v1Masks = 0xFF'FF'FF'00u;  // Select first 3 bytes
  constexpr u32 kID3v1Value = 0x54'41'47'00u;  // 'TAG\x00'
  if ((magic & kID3v1Masks) == kID3v1Value) {
    return 128;
  }

  constexpr u32 kID3v2Masks = 0xFF'FF'FF'00u;  // Select first 3 bytes
  constexpr u32 kID3v2Value = 0x49'44'33'00u;  // 'ID3\x00'
  if ((magic & kID3v2Masks) != kID3v2Value) {
    return 0;
  }

  // file = 'ID3'
  //        u8(ver_major) u8(ver_minor)
  //        u8(flags)
  //        u32(inner_tag_size)
  //        byte[inner_tag_size] id3v2 data
  //        byte[*] original_file_content
  const auto id3v2InnerTagSize = ParseID3SyncSafeInt(&buf[6]);
  if (id3v2InnerTagSize == 0) {
    return 0;
  }

  return 10 + id3v2InnerTagSize;
}

inline std::size_t GetAPEv2FullSize(u32 magic1,
                                    const u8* buf,
                                    std::size_t len) {
  u32 magic2 = umc::ReadBigEndian<u32>(&buf[4]);
  constexpr u32 kAPEv2Magic1 = 0x41'50'45'54u;  // 'APET'
  constexpr u32 kAPEv2Magic2 = 0x41'47'45'58u;  // 'AGEX'

  if (magic1 != kAPEv2Magic1 || magic2 != kAPEv2Magic2) {
    return 0;
  }

  // Tag size in bytes including footer and all tag items excluding the header.
  constexpr std::size_t kAPEv2HeaderSize = 32;
  return umc::ReadLittleEndian<u32>(&buf[0x0c]) + kAPEv2HeaderSize;
}

/**
 * @brief Detect ID3v2/APEv2 Tag size.
 *        When detected, a positive integer indicating its size from `buf`
 *        will be returned.
 *
 * @param buf
 * @param len
 * @return std::size_t
 */
inline std::size_t GetAudioHeaderMetadataSize(const u8* buf, std::size_t len) {
  // Not enough bytes to detect
  if (len < 10) {
    return 0;
  }

  u32 magic = umc::ReadBigEndian<u32>(buf);
  std::size_t id3_meta_size = GetID3HeaderSize(magic, buf, len);
  if (id3_meta_size) {
    return id3_meta_size;
  }

  // It's possible to have APEv2 header at the beginning of a file, though rare.
  std::size_t ape_meta_size = GetAPEv2FullSize(magic, buf, len);
  if (ape_meta_size) {
    return ape_meta_size;
  }

  return 0;
}

}  // namespace umc::utils
