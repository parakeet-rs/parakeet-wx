#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "test/helper.test.hh"
#include "um-crypto/decryption/tencent/JooxFileLoader.h"
#include "um-crypto/endian.h"

using ::testing::ElementsAreArray;

using namespace umc::decryption::tencent;
using namespace umc;

TEST(Joox, SimpleTest) {
  Vec<u8> test_data(test::kSize4MiB + 12 + 16 * 4);
  test::GenerateTestData(test_data, "joox test data");

  Str uuid(32, 'f');
  JooxSalt salt;

  test::GenerateTestData(uuid, "joox uuid");
  test::GenerateTestData(salt, "joox salt");

  // E!04
  WriteBigEndian(&test_data[0], u32{0x45'21'30'34});
  Arr<u8, 16> padding = {
      0xf9, 0x38, 0xbd, 0x30, 0x38, 0x46, 0x2b, 0xab,
      0x04, 0xf0, 0xd4, 0xd0, 0x71, 0x65, 0x27, 0xd4,
  };
  std::copy(padding.begin(), padding.end(),
            &test_data[12 + test::kSize1MiB + 16 * 0]);
  std::copy(padding.begin(), padding.end(),
            &test_data[12 + test::kSize2MiB + 16 * 1]);
  std::copy(padding.begin(), padding.end(),
            &test_data[12 + test::kSize3MiB + 16 * 2]);
  std::copy(padding.begin(), padding.end(),
            &test_data[12 + test::kSize4MiB + 16 * 3]);

  auto result =
      test::DecryptTestContent(JooxFileLoader::Create(uuid, salt), test_data);

  test::VerifyHash(
      result,
      "dc7be971de5af74dac95b5b16fe172ffd27a36787fac0678b9f626731b980f0d");
}
