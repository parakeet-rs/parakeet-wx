#include "../internal/endian.h"
#include "../internal/str_helper.h"
#include "um-crypto/qmcv2.h"

#include <cstdint>
#include <cstdlib>

using namespace umc;
using namespace umc::qmcv2;

QMCParseError QMCFileParser::ParseFile(QMCParsedData& result,
                                       const Vec<u8>& eof_data) {
  const auto eof_len = eof_data.size();

  if (eof_len < 4) {
    result.bytes_required = 4;
    return QMCParseError::kMoreBytesRequired;
  }

  auto eof = *reinterpret_cast<uint32_t*>(eof_data[eof_len - 4]);
  if (eof == 'QTag') {
    return ParseAndroidQTagFile(result, eof_data);
  } else if (eof == 'STag') {
    return QMCParseError::kUnsupportedFormat;
  }

  return ParseWindowsEncryptedFile(result, eof_data);
}

QMCParseError QMCFileParser::ParseAndroidQTagFile(QMCParsedData& result,
                                                  const Vec<u8>& eof_data) {
  // Legacy Android format.
  // metadata := [ansi ekey_b64] ","
  //             [ansi songid] ","
  //             [ansi metadata_version '2']
  // eof_mark := [(be)uint32_t meta_len] [bytes 'QTag']
  // qmc_file := [encrypted_data] [metadata] [eof_mark]
  const auto eof_len = eof_data.size();

  if (eof_len < 8) {
    result.bytes_required = 8;
    return QMCParseError::kMoreBytesRequired;
  }

  auto meta_len = *reinterpret_cast<const uint32_t*>(&eof_data[eof_len - 8]);
  meta_len = umc_betoh_u32(meta_len);

  size_t required_len = meta_len + 8;
  if (eof_len < required_len) {
    result.bytes_required = required_len;
    return QMCParseError::kMoreBytesRequired;
  }

  auto metadata = ParseCSVLine(&eof_data.at(eof_len - 8 - meta_len), meta_len);

  // We should see ekey_b64, song id and metadata version.
  if (metadata.size() != 3) {
    return QMCParseError::kInvalidMetadata;
  }

  // Metadata version should be 2
  if (atoi(metadata[2].c_str()) != 2) {
    return QMCParseError::kInvalidMetadata;
  }

  result.ekey_b64 = metadata[0];
  result.song_id = metadata[1];

  return QMCParseError::kOk;
}

QMCParseError QMCFileParser::ParseWindowsEncryptedFile(
    QMCParsedData& result,
    const Vec<u8>& eof_data) {
  // Legacy PC QQMusic encoded format.
  // ekey_b64 := [ansi ekey_b64]
  // eof_mark := [(le)uint32_t ekey_size]
  // qmc_file := [encrypted_data] [ekey_b64] [eof_mark]
  const auto eof_len = eof_data.size();

  if (eof_len < 4) {
    result.bytes_required = 4;
    return QMCParseError::kMoreBytesRequired;
  }

  auto ekey_size = *reinterpret_cast<const uint32_t*>(&eof_data[eof_len - 4]);
  ekey_size = umc_letoh_u32(ekey_size);

  // Currently known & supported largest key size, is 0x02C0 (704)
  if (ekey_size > 0x300) {
    return QMCParseError::kUnsupportedFormat;
  }

  const u8* eof_ekey = &eof_data[eof_len - 4];
  result.ekey_b64 = Str(eof_ekey - ekey_size, eof_ekey);
  result.song_id = Str("");

  return QMCParseError::kOk;
}
