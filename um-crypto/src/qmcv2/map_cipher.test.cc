#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "um-crypto/qmcv2.h"

using ::testing::ElementsAreArray;

using umc::qmcv2::MapCipher;
using namespace umc;

inline Vec<u8> InitKey(usize len) {
  Vec<u8> key(len);

  for (usize i = 0; i < len; i++) {
    key[i] = i & 0xff;
  }

  return key;
}

TEST(CryptoQMCv2Map, TestDecryption) {
  auto key = InitKey(140);

  auto input_buf = Vec<u8>({0, 0});
  Vec<u8> expected[5] = {Vec<u8>({0xFB, 0x81}), Vec<u8>({0x19, 0x3C}),
                         Vec<u8>({0xBB, 0xFF}), Vec<u8>({0x82, 0xFF}),
                         Vec<u8>({0x81, 0x3C})};

  auto cipher = std::make_unique<MapCipher>(key);
  for (int i = 0, offset = 1; i < 5; i++, offset *= 16) {
    cipher->Seek(offset);

    Vec<u8> result;
    ASSERT_EQ(cipher->Decrypt(result, input_buf), true);
    ASSERT_THAT(result, ElementsAreArray(expected[i]));
  }
}

TEST(CryptoQMCv2Map, TestDecryptionWithFF) {
  auto key = InitKey(140);

  auto input_buf = Vec<u8>({0xff, 0xff});
  Vec<u8> expected[5] = {Vec<u8>({0x04, 0x7E}), Vec<u8>({0xE6, 0xC3}),
                         Vec<u8>({0x44, 0x00}), Vec<u8>({0x7D, 0x00}),
                         Vec<u8>({0x7E, 0xC3})};

  auto cipher = std::make_unique<MapCipher>(key);
  for (int i = 0, offset = 1; i < 5; i++, offset *= 16) {
    cipher->Seek(offset);

    Vec<u8> result;
    ASSERT_EQ(cipher->Decrypt(result, Vec<u8>({0xff, 0xff})), true);
    ASSERT_THAT(result, ElementsAreArray(expected[i]));
  }
}

TEST(CryptoQMCv2Map, TestDecryptionAtBoundary1) {
  auto key = InitKey(140);

  auto input_buf = Vec<u8>(10);
  Vec<u8> expected =
      Vec<u8>({0x81, 0x3C, 0xBB, 0xBE, 0x82, 0x80, 0xFB, 0x81, 0x3C, 0xBB});
  Vec<u8> result;

  auto cipher = std::make_unique<MapCipher>(key);
  cipher->Seek(0x7fff - 5);
  ASSERT_EQ(cipher->Decrypt(result, input_buf), true);
  ASSERT_THAT(result, ElementsAreArray(expected));
}

TEST(CryptoQMCv2Map, TestDecryptionAtBoundary2) {
  auto key = InitKey(140);

  auto input_buf = Vec<u8>(10);
  Vec<u8> expected =
      Vec<u8>({0x81, 0x3C, 0xBB, 0xBE, 0x82, 0x7f, 0xFB, 0x81, 0x3C, 0xBB});
  Vec<u8> result;

  auto cipher = std::make_unique<MapCipher>(key);
  cipher->Seek(0x7fff * 2 - 5);
  ASSERT_EQ(cipher->Decrypt(result, input_buf), true);
  ASSERT_THAT(result, ElementsAreArray(expected));
}
