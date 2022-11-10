#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <random>
#include <vector>

#include "test/helper.test.hh"
#include "um-crypto/decryption/ximalaya/XimalayaFileLoader.h"
#include "um-crypto/endian.h"

using ::testing::ElementsAreArray;

using namespace umc::decryption::ximalaya;
using namespace umc;

TEST(Ximalaya, X3MTestCase) {
  std::vector<uint8_t> test_data(test::kSize1MiB);
  test::GenerateTestData(test_data, "x3m-test-data");

  X3MContentKey x3m_content_key;
  test::GenerateTestData(x3m_content_key, "x3m content key");

  ScrambleTable x3m_scramble_table;
  for (uint16_t i = 0; i < x3m_scramble_table.size(); i++) {
    x3m_scramble_table[i] = i;
  }

  std::vector<uint8_t> x3m_scramble_seed(x3m_scramble_table.size() * 2);
  test::GenerateTestData(x3m_scramble_seed, "x3m seed");
  for (std::size_t i = 0; i < x3m_scramble_table.size(); i++) {
    std::size_t j = ReadLittleEndian<uint16_t>(&x3m_scramble_seed[i * 2]) % x3m_scramble_table.size();
    std::swap(x3m_scramble_table[i], x3m_scramble_table[j]);
  }

  auto result = test::DecryptTestContent(XimalayaFileLoader::Create(x3m_content_key, x3m_scramble_table), test_data);

  test::VerifyHash(result, "a10bbfdcdbd388373361da6baf35c80b725f7310c3eca29d7dcf228e397a8c5a");
}
