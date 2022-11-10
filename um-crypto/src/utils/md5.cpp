#include "um-crypto/utils/md5.h"

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <span>

namespace umc::utils {

std::array<uint8_t, 16> md5(const std::span<const uint8_t> data) {
  CryptoPP::Weak::MD5 hash;
  hash.Update(data.data(), data.size());

  std::array<uint8_t, 16> digest;
  hash.Final(&digest[0]);

  return digest;
}

}  // namespace umc::utils
