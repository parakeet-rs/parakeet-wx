#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "test/helper.test.hh"
#include "um-crypto/decryption/xiami/XiamiFileLoader.h"

using ::testing::ElementsAreArray;

using namespace umc::decryption::xiami;
using namespace umc;

TEST(Xiami, SimpleCase) {
  Vec<u8> test_data(test::kSize4MiB);
  test::GenerateTestData(test_data, "xiami data");

  Vec<u8> header_override = {
      'i',  'f',  'm',  't',

      'f',  'l',  'a',  'c',

      0xfe, 0xfe, 0xfe, 0xfe,
  };
  std::copy(header_override.begin(), header_override.end(), test_data.begin());

  auto result = test::DecryptTestContent(XiamiFileLoader::Create(), test_data);

  test::VerifyHash(
      result,
      "2fa2586f0533398dc84ec2651d14efe39ad5e78b5d1983e9ce70928f94006e30");
}
