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

using ::testing::ElementsAreArray;

using namespace umc::decryption::ximalaya;
using namespace umc;

X3MContentKey kX3MContentKey = []() {
  X3MContentKey result;
  test::GenerateTestData(result, "x3m content key");
  return result;
}();

ScrambleTable kX3MScrambleTable = []() {
  ScrambleTable result;
  for (u16 i = 0; i < result.size(); i++) {
    result[i] = i;
  }
  std::mt19937 generator(0xa5ef8901);
  std::shuffle(result.begin(), result.end(), generator);
  return result;
}();

TEST(Ximalaya, X3MTestCase) {
  Vec<u8> test_data(test::kSize1MiB);

  test::GenerateTestData(test_data, "x3m-test-data");

  auto result = test::DecryptTestContent(
      XimalayaFileLoader::Create(kX3MContentKey, kX3MScrambleTable), test_data);

  test::VerifyHash(
      result,
      "77b6a7ca3e882a17cbb4f1bfe481997d54950bf4b021930a896b144586a813cc");
}
