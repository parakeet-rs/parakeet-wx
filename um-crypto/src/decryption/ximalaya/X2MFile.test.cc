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

X2MContentKey kX2MContentKey = []() {
  X2MContentKey result;
  test::GenerateTestData(result, "x2m content key");
  return result;
}();

ScrambleTable kX2MScrambleTable = []() {
  ScrambleTable result;
  for (u16 i = 0; i < result.size(); i++) {
    result[i] = i;
  }
  std::mt19937 generator(0xa5ef8901);
  std::shuffle(result.begin(), result.end(), generator);
  return result;
}();

TEST(Ximalaya, X2MTestCase) {
  Vec<u8> test_data(test::kSize1MiB);

  test::GenerateTestData(test_data, "x2m-test-data");

  auto result = test::DecryptTestContent(
      XimalayaFileLoader::Create(kX2MContentKey, kX2MScrambleTable), test_data);

  test::VerifyHash(
      result,
      "0e07e8fe105dc59f517c4b7209ee1bbd113634dafad5d2cdd6f10662ca4e3aa3");
}
