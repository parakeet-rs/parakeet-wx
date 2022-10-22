#include "um-crypto/misc/QMCFooterParser.h"

#include "um-crypto/endian.h"
#include "um-crypto/utils/DetectAudioType.h"
#include "um-crypto/utils/StringHelper.h"

#include <algorithm>

using umc::utils::ParseCSVLine;

namespace umc::misc::tencent {

namespace detail {

constexpr u32 kMagicQTag = 0x51546167;  // 'QTag'
constexpr u32 kMagicSTag = 0x53546167;  // 'STag'

class QMCFooterParserImpl : public QMCFooterParser {
 public:
  QMCFooterParserImpl(std::shared_ptr<QMCKeyDeriver> key_deriver)
      : key_deriver_(key_deriver){};

  std::unique_ptr<QMCFooterParseResult> Parse(const u8* p_in,
                                              usize len) const override {
    if (len < 4) return nullptr;

    auto eof = ReadBigEndian<u32>(&p_in[len - 4]);

    // Current (2022.04) android format, does not include embedded keys.
    if (eof == kMagicSTag) return nullptr;

    std::unique_ptr<QMCFooterParseResult> result;
    if (eof == kMagicQTag) {
      // Legacy android format, with embedded metadata & keys.
      result = ParseAndroidQTagFile(p_in, len);
    } else {
      // Windows client, end with meta size.
      result = ParseWindowsEncryptedFile(p_in, len);
    }

    // Parse Key from EKey OK
    if (result) {
      key_deriver_->FromEKey(result->key, result->ekey_b64);
    }

    return result;
  }

  inline std::unique_ptr<QMCFooterParseResult> ParseAndroidQTagFile(
      const u8* p_in,
      usize len) const {
    // Legacy Android format.
    //   metadata := [ansi ekey_b64] ","
    //               [ansi songid] ","
    //               [ansi metadata_version '2']
    //   eof_mark := [(be)uint32_t meta_len] [bytes 'QTag']
    //   qmc_file := [encrypted_data] [metadata] [eof_mark]
    //
    // Where:
    //   meta_len := bytes( [metadata] [eof_mark] ).size()
    if (len < 8) return nullptr;

    auto meta_len = ReadBigEndian<u32>(&p_in[len - 8]);

    size_t required_len = meta_len + 8;
    if (len < required_len) return nullptr;

    auto result = std::make_unique<QMCFooterParseResult>();
    result->eof_bytes_ignore = required_len;

    auto metadata = ParseCSVLine(&p_in[len - 8 - meta_len], meta_len);

    // We should see the following:
    //    ekey_b64, song id and metadata version;
    // where metadata version should be '2'.
    if (metadata.size() != 3 || metadata[2] != "2") return nullptr;

    result->ekey_b64 = metadata[0];
    return result;
  }

  inline std::unique_ptr<QMCFooterParseResult> ParseWindowsEncryptedFile(
      const u8* p_in,
      usize len) const {
    // Legacy PC QQMusic encoded format.
    // ekey_b64 := [ansi ekey_b64]
    // eof_mark := [(le)uint32_t ekey_size]
    // qmc_file := [encrypted_data] [ekey_b64] [eof_mark]
    if (len < 4) return nullptr;

    auto ekey_size = ReadLittleEndian<u32>(&p_in[len - 4]);

    size_t required_len = ekey_size + 4;
    if (len < required_len) return nullptr;

    auto result = std::make_unique<QMCFooterParseResult>();
    result->eof_bytes_ignore = required_len;

    const u8* eof_ekey = &p_in[len - 4];
    const std::string ekey_b64 = Str(eof_ekey - ekey_size, eof_ekey);

    result->ekey_b64 = ekey_b64;
    return result;
  }

 private:
  std::shared_ptr<QMCKeyDeriver> key_deriver_;
};

}  // namespace detail

std::unique_ptr<QMCFooterParser> QMCFooterParser::Create(
    std::shared_ptr<QMCKeyDeriver> key_deriver) {
  return std::make_unique<detail::QMCFooterParserImpl>(key_deriver);
}

}  // namespace umc::misc::tencent
