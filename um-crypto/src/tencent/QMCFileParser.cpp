#include "../internal/StringHelper.h"
#include "um-crypto/endian.h"
#include "um-crypto/qmcv2.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>

using namespace umc;
using namespace umc::qmcv2;

const u32 kMagicQTag = 0x51546167;  // 'QTag'
const u32 kMagicSTag = 0x53546167;  // 'STag'

// "UVF..." = base64_encode("QQMusic EncV2,Key:")
// Found in ekey section of mflac.
const char* kQQMusicEncV2KeyHeader = "UVFNdXNpYyBFbmNWMixLZXk6";

QMCParseError QMCFileParser::ParseFile(QMCParsedData& result,
                                       const Vec<u8>& eof_data) {
  const auto eof_len = eof_data.size();

  if (eof_len < 4) {
    result.bytes_required = 4;
    return QMCParseError::kMoreBytesRequired;
  }

  auto eof = ReadBEU32(&eof_data[eof_len - 4]);
  if (eof == kMagicQTag) {
    return ParseAndroidQTagFile(result, eof_data);
  } else if (eof == kMagicSTag) {
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

  auto meta_len = ReadBEU32(&eof_data[eof_len - 8]);

  size_t required_len = meta_len + 8;
  if (eof_len < required_len) {
    result.bytes_required = required_len;
    return QMCParseError::kMoreBytesRequired;
  }
  result.eof_bytes_ignore = required_len;

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

  auto ekey_size = ReadLEU32(&eof_data[eof_len - 4]);
  ekey_size = umc_letoh_u32(ekey_size);

  // Currently known & supported largest key size, is 0x02C0 (704)
  if (ekey_size > 0x300) {
    return QMCParseError::kUnsupportedFormat;
  }

  size_t required_len = ekey_size + 4;
  if (eof_len < required_len) {
    result.bytes_required = required_len;
    return QMCParseError::kMoreBytesRequired;
  }
  result.eof_bytes_ignore = required_len;

  const u8* eof_ekey = &eof_data[eof_len - 4];
  const std::string ekey_b64 = Str(eof_ekey - ekey_size, eof_ekey);

  // Unsupported, key derivation has changed.
  if (ekey_b64.starts_with(kQQMusicEncV2KeyHeader)) {
    return QMCParseError::kUnsupportedFormat;
  }

  result.ekey_b64 = ekey_b64;
  result.song_id = Str("");

  return QMCParseError::kOk;
}
