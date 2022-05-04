#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "um-crypto/qmcv2.h"

using ::testing::ElementsAreArray;
using namespace umc;
using namespace umc::tencent;

class SimpleEKeyDeriveFixture : public SimpleEKeyDerive {
 public:
  SimpleEKeyDeriveFixture(u8 key) : SimpleEKeyDerive(key){};

  void MakeSimpleKey(Vec<u8>& out) const {
    SimpleEKeyDerive::MakeSimpleKey(out);
  }
};

TEST(SimpleEKeyDerive, MakeSimpleKey) {
  Vec<u8> buf(8);
  SimpleEKeyDeriveFixture deriver(0b10101010);
  deriver.MakeSimpleKey(buf);

  uint8_t expected[8] = {0x24, 0x30, 0x3D, 0x4C, 0x5D, 0x72, 0x8D, 0xAF};
  ASSERT_THAT(buf, ElementsAreArray(expected));
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
