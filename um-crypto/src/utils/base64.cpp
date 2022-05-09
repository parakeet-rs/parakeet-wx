#include "um-crypto/utils/base64.h"
#include "um-crypto/utils/StringHelper.h"

#include <boost/beast/core/detail/base64.hpp>

namespace base64 = boost::beast::detail::base64;

namespace umc::utils {

Vec<u8> Base64Decode(const Str& input) {
  std::string input_clean = RemoveWhitespace(input);
  Vec<u8> buf(base64::decoded_size(input_clean.size()));

  const auto bytes_written =
      base64::decode(buf.data(), input_clean.data(), input_clean.size()).first;
  buf.resize(bytes_written);

  return buf;
}

Str Base64Encode(const Vec<u8>& input) {
  Str buf;
  buf.resize(base64::encoded_size(input.size()));

  const auto bytes_written =
      base64::encode(buf.data(), input.data(), input.size());
  buf.resize(bytes_written);

  return buf;
}

}  // namespace umc::utils
