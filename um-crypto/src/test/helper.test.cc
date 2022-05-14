#include "helper.test.hh"

using namespace umc;

TEST(TestHelper, VerifyHash) {
  test::VerifyHash(
      "Parakeet", 8,
      "e6d539d8f612fe194b15aabb195a896b9dbe3eb4fd49dc0ae08d9740ef215a7b");
}

TEST(TestHelper, GenerateTestData) {
  // it should generate the same data
  auto test_data1 = test::GenerateTestData(256, "test_data_stable");
  test::VerifyHash(
      test_data1,
      "cdffdcbb64563d64c7d56cf02dfe8f2642d9075ff6215aa61378541617ab6cb3");
}
