#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "um-crypto/qmcv2.h"

using ::testing::ElementsAreArray;

TEST(qmcv2_map, TestDecryption) {
  uint8_t key[140];
  for (int i = 0; i < 140; i++) {
    key[i] = uint8_t(i & 0xff);
  }

  uint8_t buf[10];
  uint8_t expect_encrypted[10] = {0x37, 0x4D, 0xD5, 0xF0, 0x77,
                                  0x33, 0x4E, 0x33, 0x4D, 0xF0};

  auto qmc = qmcv2_map_new(key, sizeof(key) / sizeof(key[0]));
  for (int i = 0, offset = 1; i < 10; i += 2, offset *= 16) {
    qmcv2_map_decrypt(qmc, &buf[i], 2, offset);
  }
  qmcv2_map_free(qmc);

  ASSERT_THAT(buf, ElementsAreArray(expect_encrypted));
}

TEST(qmcv2_map, TestDecryptionWithFF) {
  uint8_t key[140];
  for (int i = 0; i < 140; i++) {
    key[i] = uint8_t(i & 0xff);
  }

  uint8_t buf[10];
  memset(buf, 0xff, sizeof(buf));

  uint8_t expect_encrypted[10] = {0x04, 0x7E, 0xE6, 0xC3, 0x44,
                                  0x00, 0x7D, 0x00, 0x7E, 0xC3};

  auto qmc = qmcv2_map_new(key, sizeof(key) / sizeof(key[0]));
  for (int i = 0, offset = 1; i < 10; i += 2, offset *= 16) {
    qmcv2_map_decrypt(qmc, &buf[i], 2, offset);
  }
  qmcv2_map_free(qmc);

  ASSERT_THAT(buf, ElementsAreArray(expect_encrypted));
}
