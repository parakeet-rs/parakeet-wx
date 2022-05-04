#pragma once

#include "um-crypto/types.h"

#include <utility>

namespace umc::qmcv2 {
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

class QMCFileParser {
 public:
  QMCFileParser(){};
  virtual ~QMCFileParser(){};

  static QMCParseError ParseFile(QMCParsedData& result,
                                 const Vec<u8>& eof_data);
  static QMCParseError ParseAndroidQTagFile(QMCParsedData& result,
                                            const Vec<u8>& eof_data);
  static QMCParseError ParseWindowsEncryptedFile(QMCParsedData& result,
                                                 const Vec<u8>& eof_data);
};

}  // namespace umc::qmcv2
