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
  std::vector<uint8_t> test_data(test::kSize4MiB + 12 + 16 * 4);
  test::GenerateTestData(test_data, "joox test data");

  std::string uuid(32, 'f');
  JooxSalt salt;

  test::GenerateTestData(uuid, "joox uuid");
  test::GenerateTestData(salt, "joox salt");

  // E!04
  WriteBigEndian(&test_data[0], uint32_t{0x45'21'30'34});
  std::array<uint8_t, 16> padding = {
      0xf9, 0x38, 0xbd, 0x30, 0x38, 0x46, 0x2b, 0xab, 0x04, 0xf0, 0xd4, 0xd0, 0x71, 0x65, 0x27, 0xd4,
  };

  for (std::size_t i = 1; i <= 4; i++) {
    std::copy(padding.begin(), padding.end(), &test_data[12 + test::kSize1MiB * i + 16 * i - 16]);
  }
  test::VerifyHash(test_data, "684e32738bd84dc95143df5657d02498389516328e50d0b8492848d6e245def1");

  auto result = test::DecryptTestContent(JooxFileLoader::Create(uuid, salt), test_data);

  test::VerifyHash(result, "dc7be971de5af74dac95b5b16fe172ffd27a36787fac0678b9f626731b980f0d");
}
