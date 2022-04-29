#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "um-crypto/qmcv2.h"

using ::testing::ElementsAreArray;

void make_simple_key(uint8_t* buf, uint8_t seed, size_t len);
TEST(umc_qmcv2_key_derive, make_simple_key) {
  uint8_t buf[8];
  make_simple_key(buf, 106, 8);

  uint8_t expected[8] = {0x69, 0x56, 0x46, 0x38, 0x2b, 0x20, 0x15, 0x0b};
  ASSERT_THAT(buf, ElementsAreArray(expected));
}

void derive_tea_key(uint8_t* tea_key, const uint8_t* ekey);
TEST(umc_qmcv2_key_derive, derive_tea_key) {
  uint8_t ekey[] = {0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8};
  uint8_t expected[] = {0x69, 0xf1, 0x56, 0xf2, 0x46, 0xf3, 0x38, 0xf4,
                        0x2b, 0xf5, 0x20, 0xf6, 0x15, 0xf7, 0x0b, 0xf8};

  uint8_t tea_key[16];
  derive_tea_key(tea_key, ekey);
  ASSERT_THAT(tea_key, ElementsAreArray(expected));
}

TEST(umc_qmcv2_key_derive, parse_ekey_b64) {
  const char* expected_key = "This is a test key for test purpose :D";
  const char* ekey =
      "VGhpcyBpcyBHFWEh4cjZ1Vi7rJ56XeoPlqGM1sxBGPg7mt89umKclFBr9iqfmFdS";

  uint8_t* decoded;
  size_t decode_len;
  umc_qmcv2_derive_from_ekey_b64(decoded, decode_len, ekey);

  ASSERT_EQ(strlen((char*)decoded), decode_len);
  ASSERT_STREQ((char*)decoded, expected_key);
}
