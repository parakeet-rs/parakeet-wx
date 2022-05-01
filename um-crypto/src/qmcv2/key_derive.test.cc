#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "um-crypto/qmcv2.h"

using ::testing::ElementsAreArray;
using namespace umc;
using namespace umc::qmcv2;

class SimpleEKeyDeriveFixture : public SimpleEKeyDerive {
 public:
  SimpleEKeyDeriveFixture(u8 key) : SimpleEKeyDerive(key){};

  using SimpleEKeyDerive::DeriveTEAKey;
  using SimpleEKeyDerive::MakeSimpleKey;
};

TEST(SimpleEKeyDerive, MakeSimpleKey) {
  Vec<u8> buf(8);
  SimpleEKeyDeriveFixture deriver(0b10101010);
  deriver.MakeSimpleKey(buf);

  uint8_t expected[8] = {0x24, 0x30, 0x3D, 0x4C, 0x5D, 0x72, 0x8D, 0xAF};
  ASSERT_THAT(buf, ElementsAreArray(expected));
}

TEST(SimpleEKeyDerive, DeriveTEAKey) {
  Vec<u8> expected({0x24, 0xF1, 0x30, 0xF2, 0x3D, 0xF3, 0x4C, 0xF4, 0x5D, 0xF5,
                    0x72, 0xF6, 0x8D, 0xF7, 0xAF, 0xF8});
  Vec<u8> ekey_header({0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8});
  SimpleEKeyDeriveFixture deriver(0b10101010);

  auto result = deriver.DeriveTEAKey(ekey_header);

  ASSERT_THAT(result, ElementsAreArray(expected));
}

TEST(SimpleEKeyDerive, FromEKey) {
  const char p[] = "This is a test key for test purpose :D";
  Vec<u8> expected(p, p + strlen(p));
  Vec<u8> result;

  SimpleEKeyDeriveFixture deriver(0b10101010);
  Str ekey("VGhpcyBpcyA8lAKKjW/mCQmGAnFlD2mtubAZxC5JQNFKn2uy6ev9NPulvo04Rg7e");
  ASSERT_EQ(deriver.FromEKey(result, ekey), true);

  ASSERT_THAT(result, ElementsAreArray(expected));
}
