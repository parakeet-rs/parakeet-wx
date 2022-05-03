#include "um-crypto/kugou/KGMMaskGenerator.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>

using ::testing::ElementsAreArray;
using namespace umc;
using namespace umc::kugou;

KugouTable GenerateTable(u8 param) {
  KugouTable t;
  for (int i = 0; i < kTableSize; i++) {
    t[i] = u8(i) ^ param;
  }
  return t;
}

TEST(KGMMaskGenerator, FetchMask) {
  KugouTable t1 = GenerateTable(0xff);
  KugouTable t2 = GenerateTable(0x33);
  KugouTable v2 = GenerateTable(0x55);
  memset(t1.data(), 0, 16);
  memset(t2.data(), 0, 16);

  Vec<u8> buf(5);
  auto expected_1 = Vec<u8>({0x55, 0x54, 0x57, 0x56, 0x51});
  auto expected_2 = Vec<u8>({0x5b, 0x5a, 0xbb, 0xba, 0xb9});

  auto gen = std::make_unique<KGMMaskGenerator>();
  gen->SetTable(t1, t2, v2);

  gen->FetchMask(buf.data(), buf.size(), 0);
  ASSERT_THAT(buf, ElementsAreArray(expected_1));

  gen->FetchMask(buf.data(), buf.size(), kTableSize - 2);
  ASSERT_THAT(buf, ElementsAreArray(expected_2));
}
