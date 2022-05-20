#include "test/helper.test.hh"

#include "um-crypto/decryption/kugou/KugouFileLoader.h"

using namespace umc::decryption;
using namespace umc;

TEST(KugouFileLoader, KGM1) {
  kugou::KugouInternalTable t1{};
  kugou::KugouInternalTable t2{};
  kugou::KugouInternalTable v2{};

  test::GenerateTestData(t1, "kgm_test1_t1");
  test::GenerateTestData(t2, "kgm_test1_t2");
  test::GenerateTestData(v2, "kgm_test1_v2");

  Vec<u8> test_data(test::kSize4MiB);
  test::GenerateTestData(test_data, "kgm_test1_data");
  Arr<u8, 20> header_override = {
      0x7C, 0xD5, 0x32, 0xEB, 0x86, 0x02, 0x7F, 0x4B, 0xA8, 0xAF,
      0xA6, 0x8E, 0x0F, 0xFF, 0x99, 0x14, 0x00, 0x04, 0x00, 0x00,
  };
  std::copy(header_override.begin(), header_override.end(), test_data.begin());

  auto result = test::DecryptTestContent(
      kugou::KugouFileLoader::Create(t1, t2, v2, false), test_data);

  test::VerifyHash(
      result,
      "33db227aaa95a9a1ed0818e1c67bb6f99ccaacde436bedf0e69ea6eb49f864fb");
}
