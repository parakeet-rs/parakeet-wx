#pragma once

#include "QMCKeyDeriver.h"

#include <memory>

namespace umc::misc::tencent {

struct QMCFooterParseResult {
  std::string ekey_b64;
  std::vector<uint8_t> key;
  std::size_t eof_bytes_ignore;
};

class QMCFooterParser {
 public:
  /**
   * @brief Parse a given block of footer data.
   *
   * @param p_in Data pointer
   * @param len  Size of the data
   * @return std::unique_ptr<QMCFooterParseResult>
   * @return nullptr - Could not parse / not enough data
   */
  virtual std::unique_ptr<QMCFooterParseResult> Parse(const uint8_t* p_in, std::size_t len) const = 0;

  static std::unique_ptr<QMCFooterParser> Create(std::shared_ptr<QMCKeyDeriver> key_deriver);
};

}  // namespace umc::misc::tencent
