#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tc_tea.h"

#include <vector>

using ::testing::ElementsAreArray;
using namespace umc;

// Demonstrate some basic assertions.
TEST(umc_misc_tc_tea_cbc, BasicDecryptionTest) {
  u8 encrypted[] = {0x91, 0x09, 0x51, 0x62, 0xe3, 0xf5, 0xb6, 0xdc,
                    0x6b, 0x41, 0x4b, 0x50, 0xd1, 0xa5, 0xb8, 0x4e,
                    0xc5, 0x0d, 0x0c, 0x1b, 0x11, 0x96, 0xfd, 0x3c};
  u8 encryption_key[16] = {'1', '2', '3', '4', '5', '6', '7', '8',
                           'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  u8 good_decrypted_data[] = {1, 2, 3, 4, 5, 6, 7, 8};

  Vec<u8> decrypted(24);
  size_t decrypted_len = decrypted.size();
  misc::tc_tea::cbc::Decrypt(decrypted.data(), decrypted_len, encrypted,
                             sizeof(encrypted), encryption_key);
  decrypted.resize(decrypted_len);
  ASSERT_THAT(decrypted, ElementsAreArray(good_decrypted_data))
      << "tc_tea_cbc test decryption failed: data mismatch";
}

// Demonstrate some basic assertions.
TEST(umc_misc_tc_tea_cbc, BasicEncryptionTest) {
  u8 encryption_key[16] = {'1', '2', '3', '4', '5', '6', '7', '8',
                           'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  u8 good_decrypted_data[] = {1, 2, 3, 4, 5, 6, 7, 8};

  Arr<u8, 8> plain_data = {1, 2, 3, 4, 5, 6, 7, 8};
  Vec<u8> encrypted(misc::tc_tea::cbc::GetEncryptedSize(plain_data.size()));
  usize encrypted_size = encrypted.size();
  misc::tc_tea::cbc::Encrypt(encrypted.data(), encrypted_size,
                             plain_data.data(), plain_data.size(),
                             encryption_key);
  encrypted.resize(encrypted_size);

  Vec<u8> decrypted(24);
  size_t decrypted_len;
  misc::tc_tea::cbc::Decrypt(decrypted.data(), decrypted_len, encrypted.data(),
                             encrypted.size(), encryption_key);
  decrypted.resize(decrypted_len);
  ASSERT_THAT(decrypted, ElementsAreArray(good_decrypted_data))
      << "tc_tea_cbc test encryption/decryption failed: data mismatch";
}
