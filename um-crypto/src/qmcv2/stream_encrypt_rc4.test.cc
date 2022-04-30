#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "um-crypto/qmcv2.h"

using ::testing::ElementsAreArray;

TEST(qmcv2_rc4, TestDecryption) {
  uint8_t key[400];
  for (int i = 0; i < 400; i++) {
    key[i] = uint8_t(i & 0xff);
  }

  uint8_t buf[10];
  memset(buf, 0, sizeof(buf));
  uint8_t expect_decrypted[10] = {0x32, 0x10, 0x00, 0x00, 0xC2,
                                  0x33, 0xF9, 0xFE, 0xF9, 0xFE};

  auto qmc = umc::qmc::v2::rc4::new_from_key(key, sizeof(key) / sizeof(key[0]));
  for (int i = 0, offset = 1; i < 10; i += 2, offset *= 16) {
    umc::qmc::v2::rc4::decrypt(qmc, &buf[i], 2, offset);
  }
  umc::qmc::v2::rc4::free(qmc);

  ASSERT_THAT(buf, ElementsAreArray(expect_decrypted));
}

TEST(qmcv2_rc4, TestDecryptionWithFF) {
  uint8_t key[400];
  for (int i = 0; i < 400; i++) {
    key[i] = uint8_t(i & 0xff);
  }

  uint8_t buf[10];
  memset(buf, 0xff, sizeof(buf));

  uint8_t expect_decrypted[10] = {0xCD, 0xEF, 0xFF, 0xFF, 0x3D,
                                  0xCC, 0x06, 0x01, 0x06, 0x01};

  auto qmc = umc::qmc::v2::rc4::new_from_key(key, sizeof(key) / sizeof(key[0]));
  for (int i = 0, offset = 1; i < 10; i += 2, offset *= 16) {
    umc::qmc::v2::rc4::decrypt(qmc, &buf[i], 2, offset);
  }
  umc::qmc::v2::rc4::free(qmc);

  ASSERT_THAT(buf, ElementsAreArray(expect_decrypted));
}

TEST(qmcv2_rc4, TestDecryptionWithFFAtFirstSegmentEndBoundary) {
  uint8_t key[400];
  for (int i = 0; i < 400; i++) {
    key[i] = uint8_t(i & 0xff);
  }

  uint8_t buf[10];
  memset(buf, 0xff, sizeof(buf));

  uint8_t expect_decrypted[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                  0x8C, 0x77, 0xE0, 0x7F, 0xB9};

  auto qmc = umc::qmc::v2::rc4::new_from_key(key, sizeof(key) / sizeof(key[0]));
  umc::qmc::v2::rc4::decrypt(qmc, buf, 10, 0x80 - 5);
  umc::qmc::v2::rc4::free(qmc);

  ASSERT_THAT(buf, ElementsAreArray(expect_decrypted));
}

TEST(qmcv2_rc4, TestDecryptionWithFFAtSecondSegmentEndBoundary) {
  uint8_t key[400];
  for (int i = 0; i < 400; i++) {
    key[i] = uint8_t(i & 0xff);
  }

  uint8_t buf[10];
  memset(buf, 0xff, sizeof(buf));

  uint8_t expect_decrypted[10] = {0x98, 0x70, 0xE6, 0xF5, 0xB4,
                                  0xE5, 0x7B, 0xCC, 0xEA, 0x15};

  auto qmc = umc::qmc::v2::rc4::new_from_key(key, sizeof(key) / sizeof(key[0]));
  umc::qmc::v2::rc4::decrypt(qmc, buf, 10, 0x1400 - 5);
  umc::qmc::v2::rc4::free(qmc);

  ASSERT_THAT(buf, ElementsAreArray(expect_decrypted));
}
