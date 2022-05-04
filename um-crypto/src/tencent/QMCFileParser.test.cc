#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstdint>
#include <vector>

#include "um-crypto/tencent/QMCFileParser.h"

using ::testing::ElementsAreArray;
using namespace umc;
using namespace umc::qmcv2;

TEST(QMCFileParser, ParseWindowsEncryptedFile) {
  QMCParsedData result;
  Vec<u8> file({0xff, 'g', 'o', '!', 0x03, 0, 0, 0});
  ASSERT_EQ(QMCFileParser::ParseWindowsEncryptedFile(result, file),
            QMCParseError::kOk);

  ASSERT_STREQ(result.song_id.c_str(), "");
  ASSERT_STREQ(result.ekey_b64.c_str(), "go!");
}

TEST(QMCFileParser, ParseAndroidQTagFile_ShouldAccept) {
  QMCParsedData result;
  Vec<u8> file({0xff, 'm',  'o',  'c',  'k', '_', 'e', 'k',
                'e',  'y',  ',',  '6',  '4', '2', ',', '2',
                0x00, 0x00, 0x00, 0x0f, 'Q', 'T', 'a', 'g'});
  ASSERT_EQ(QMCFileParser::ParseAndroidQTagFile(result, file),
            QMCParseError::kOk);

  ASSERT_STREQ(result.song_id.c_str(), "642");
  ASSERT_STREQ(result.ekey_b64.c_str(), "mock_ekey");
}

TEST(QMCFileParser, ParseAndroidQTagFile_ShouldRejectInvalidMeta) {
  QMCParsedData result;
  ASSERT_EQ(QMCFileParser::ParseAndroidQTagFile(
                result, {0xff, 'm',  'o',  'c',  'k', '_', 'e', 'k',
                         'e',  'y',  ',',  '6',  '4', '2', ',', '0',
                         0x00, 0x00, 0x00, 0x0f, 'Q', 'T', 'a', 'g'}),
            QMCParseError::kInvalidMetadata);
}

TEST(QMCFileParser, ParseAndroidQTagFile_ShouldRejectMoreBytesRequired) {
  QMCParsedData result;

  ASSERT_EQ(QMCFileParser::ParseAndroidQTagFile(result, {'Q', 'T', 'a', 'g'}),
            QMCParseError::kMoreBytesRequired);
  ASSERT_EQ(result.bytes_required, 8);

  ASSERT_EQ(QMCFileParser::ParseAndroidQTagFile(
                result, {0, 0, 0, 1, 'Q', 'T', 'a', 'g'}),
            QMCParseError::kMoreBytesRequired);
  ASSERT_EQ(result.bytes_required, 9);

  ASSERT_EQ(QMCFileParser::ParseAndroidQTagFile(
                result, {0xff, 0, 0, 0, 2, 'Q', 'T', 'a', 'g'}),
            QMCParseError::kMoreBytesRequired);
  ASSERT_EQ(result.bytes_required, 10);
}
