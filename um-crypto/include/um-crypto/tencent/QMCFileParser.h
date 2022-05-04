#pragma once

#include "um-crypto/types.h"
#include "um-crypto/utils/DetectAudioType.h"

#include <utility>

namespace umc::tencent {
enum class QMCParseError {
  kOk = 0,
  kInvalidMetadata,
  kMoreBytesRequired,
  kUnsupportedFormat,
  kUnknownFileType,
};

struct QMCParsedData {
  Str song_id;
  Str ekey_b64;

  // numbe of bytes to ignore at the end of file.
  usize eof_bytes_ignore;

  // Error report
  usize bytes_required;
};

constexpr usize kLegacyQMCDetectionSize = utils::kAudioTypeSniffBufferSize;
class QMCFileParser {
 public:
  QMCFileParser(){};
  virtual ~QMCFileParser(){};

  static bool IsLegacyQMCFile(const Vec<u8>& bof_data);

  static QMCParseError ParseFile(QMCParsedData& result,
                                 const Vec<u8>& eof_data);
  static QMCParseError ParseAndroidQTagFile(QMCParsedData& result,
                                            const Vec<u8>& eof_data);
  static QMCParseError ParseWindowsEncryptedFile(QMCParsedData& result,
                                                 const Vec<u8>& eof_data);
};

}  // namespace umc::tencent
