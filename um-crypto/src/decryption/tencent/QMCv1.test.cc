#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "test/helper.test.hh"
#include "um-crypto/decryption/tencent/QMCv1Loader.h"

using ::testing::ElementsAreArray;

using namespace umc::decryption::tencent;
using namespace umc;

TEST(QMCv1, StaticCipher) {
  std::vector<uint8_t> test_data(test::kSize4MiB);
  test::GenerateTestData(test_data, "qmcv1 static data");

  QMCv1Key key(256);
  test::GenerateTestData(key, "qmcv1 static key");

  auto result = test::DecryptTestContent(QMCv1Loader::Create(key), test_data);

  test::VerifyHash(result, "2f9c936ed130a654911e0e2bc872fec33c90288e78df2a0aa41d352164c3b4e3");
}

class QMCFooterParserMock : public umc::misc::tencent::QMCFooterParser {
 private:
  std::vector<uint8_t> key_;

 public:
  QMCFooterParserMock(std::vector<uint8_t> key) : key_(key) {}

  std::unique_ptr<umc::misc::tencent::QMCFooterParseResult> Parse(const uint8_t* p_in, std::size_t len) const override {
    auto result = std::make_unique<umc::misc::tencent::QMCFooterParseResult>();
    result->eof_bytes_ignore = 0;
    result->key = key_;
    return result;
  }
};

TEST(QMCv1, MapCipher) {
  std::vector<uint8_t> test_data(test::kSize4MiB);
  test::GenerateTestData(test_data, "qmcv1 map cipher data");

  QMCv1Key key(256);
  test::GenerateTestData(key, "qmcv1 map cipher derived key");

  auto qmc_footer_parser = std::make_shared<QMCFooterParserMock>(key);
  auto result = test::DecryptTestContent(QMCv1Loader::Create(qmc_footer_parser), test_data);

  test::VerifyHash(result, "ce84e9ac24ef5b2f02a11f74ffa8eb7008fe2898855617596c5ee217139fc214");
}
