#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tc_tea_cbc.h"

#include <cstdint>
#include <vector>

using ::testing::ElementsAreArray;

// Demonstrate some basic assertions.
TEST(umc_tc_tea_ecb, BasicDecryption) {
  uint8_t good_data[] = {0x91, 0x09, 0x51, 0x62, 0xe3, 0xf5, 0xb6, 0xdc,
                         0x6b, 0x41, 0x4b, 0x50, 0xd1, 0xa5, 0xb8, 0x4e,
                         0xc5, 0x0d, 0x0c, 0x1b, 0x11, 0x96, 0xfd, 0x3c};
  uint8_t encryption_key[16] = {'1', '2', '3', '4', '5', '6', '7', '8',
                                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  uint8_t good_decrypted_data[] = {1, 2, 3, 4, 5, 6, 7, 8};

  uint8_t* plain = static_cast<uint8_t*>(malloc(24));
  size_t plain_len;
  umc::tc_tea::cbc_decrypt(plain, plain_len, good_data, sizeof(good_data),
                           encryption_key);
  std::vector<uint8_t> plain_vec(plain, plain + plain_len);
  ASSERT_THAT(plain_vec, ElementsAreArray(good_decrypted_data));
}
