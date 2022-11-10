#pragma once
#include "um-crypto/decryption/DecryptionStream.h"
#include "um-crypto/types.h"
#include "um-crypto/utils/StringHelper.h"
#include "um-crypto/utils/hex.h"

#include <cryptopp/sha.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>

namespace umc::test {
using namespace ::testing;

constexpr std::size_t kSize1MiB = 1 * 1024 * 1024;
constexpr std::size_t kSize2MiB = 2 * kSize1MiB;
constexpr std::size_t kSize3MiB = 3 * kSize1MiB;
constexpr std::size_t kSize4MiB = 4 * kSize1MiB;
constexpr std::size_t kSize32MiB = 8 * kSize4MiB;

typedef std::array<u8, 256 / 8> Hash_SHA256;

/**
 * @brief Deterministic random data generator
 * It should meet the following criteria:
 * - Fast
 * - Deterministic
 * - Stable
 *
 * It does not have to be secure, as the data generated are for
 *   test purpose only.
 *
 * @param len
 * @param unique_name
 * @return std::vector<u8>
 */
inline void GenerateTestData(u8* out,
                             std::size_t len,
                             const std::string& unique_name) {
  u8 S[256];

  /* init seedbox */ {
    auto key = reinterpret_cast<const u8*>(unique_name.c_str());
    auto key_len = std::max(unique_name.size(), std::size_t{1});

    for (std::size_t i = 0; i < 256; i++) {
      S[i] = u8(i);
    }

    u8 j = 0;
    for (std::size_t i = 0; i < 256; i++) {
      j += S[i] + key[i % key_len];
      std::swap(S[i], S[j]);
    }
  }

  u8 x = 0;
  u8 y = 0;
  for (std::size_t i = 0; i < len; i++) {
    x += 1;
    y += S[x];
    std::swap(S[x], S[y]);
    out[i] = S[u8(S[x] + S[y])];
  }
}

inline std::vector<u8> GenerateTestData(std::size_t len,
                                        const std::string& unique_name) {
  std::vector<u8> result(len);
  GenerateTestData(result.data(), len, unique_name);
  return result;
}

template <std::size_t Size>
inline void GenerateTestData(std::array<u8, Size>& out,
                             const std::string& unique_name) {
  GenerateTestData(out.data(), out.size(), unique_name);
}
inline void GenerateTestData(std::vector<u8>& out,
                             const std::string& unique_name) {
  GenerateTestData(out.data(), out.size(), unique_name);
}
inline void GenerateTestData(std::string& out, const std::string& unique_name) {
  GenerateTestData(reinterpret_cast<u8*>(out.data()), out.size(), unique_name);
}

inline void VerifyHash(const void* data,
                       std::size_t len,
                       const Hash_SHA256& expect_hash) {
  CryptoPP::SHA256 sha256;
  sha256.Update(reinterpret_cast<const u8*>(data), len);
  Hash_SHA256 actual_hash;
  ASSERT_EQ(actual_hash.size(), sha256.DigestSize()) << "hash size mismatch";
  sha256.Final(actual_hash.data());

  std::vector<u8> actual_hash_vec(actual_hash.begin(), actual_hash.end());
  std::vector<u8> expect_hash_vec(expect_hash.begin(), expect_hash.end());
  ASSERT_THAT(utils::Hex(actual_hash_vec), StrEq(utils::Hex(expect_hash_vec)));
}

inline void VerifyHash(const void* data,
                       std::size_t len,
                       const std::string& hash) {
  auto hash_bytes = utils::Unhex(hash);
  Hash_SHA256 hash_array;
  ASSERT_EQ(hash_array.size(), hash_bytes.size())
      << "parsed hash [" << hash << "] does not match SHA256 digest size.";
  std::copy(hash_bytes.begin(), hash_bytes.end(), hash_array.begin());
  VerifyHash(data, len, hash_array);
}

inline void VerifyHash(const std::vector<u8>& in,
                       const Hash_SHA256& expect_hash) {
  VerifyHash(in.data(), in.size(), expect_hash);
}

inline void VerifyHash(const std::vector<u8>& in,
                       const std::string& expect_hash) {
  VerifyHash(in.data(), in.size(), expect_hash);
}

template <std::size_t Size>
inline void VerifyHash(const std::array<u8, Size>& in,
                       const Hash_SHA256& expect_hash) {
  VerifyHash(in.data(), in.size(), expect_hash);
}

template <std::size_t Size>
inline void VerifyHash(const std::array<u8, Size>& in,
                       const std::string& expect_hash) {
  VerifyHash(in.data(), in.size(), expect_hash);
}

template <class Loader>
inline std::vector<u8> DecryptTestContent(std::unique_ptr<Loader> loader,
                                          const std::vector<u8>& test_data) {
  umc::decryption::DetectionBuffer footer;

  if (test_data.size() < footer.size()) {
    throw std::runtime_error("not enough data to init from footer");
  }

  std::copy_n(&test_data[test_data.size() - footer.size()], footer.size(),
              footer.begin());
  std::size_t reserved_size = loader->InitWithFileFooter(footer);

  if (!loader->Write(test_data.data(), test_data.size() - reserved_size)) {
    auto err = loader->GetErrorMessage();
    throw std::runtime_error(
        utils::Format("invoke DecryptionStream::Write failed, error: %s",
                      loader->GetErrorMessage().c_str()));
  }

  if (loader->InErrorState()) {
    throw std::runtime_error(
        utils::Format("error from DecryptionStream::InErrorState: %s",
                      loader->GetErrorMessage().c_str()));
  }

  std::vector<u8> result;
  loader->ReadAll(result);
  return result;
}

}  // namespace umc::test
