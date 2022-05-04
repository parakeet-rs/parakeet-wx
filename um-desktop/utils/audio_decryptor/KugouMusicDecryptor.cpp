#include "KugouMusicDecryptor.h"

#include <um-crypto/kugou.h>

using namespace umc;

namespace umd::utils::audio_decryptor {

constexpr usize kInitialDetectionSize = 100;

bool KugouMusicDecryptor::SetupDecryptor() {
  if (file_size_ < kInitialDetectionSize) {
    return false;
  }

  Vec<u8> fileHeader(kInitialDetectionSize);

  kugou::KGMParseError error;
  kugou::KGMParseResult result;
  result.bytes_required = kInitialDetectionSize;

  do {
    const usize header_size = result.bytes_required;
    fileHeader.resize(header_size);
    in_file_.seekg(0, std::ifstream::beg);
    Read(fileHeader.data(), header_size);
    error = kugou::KGMFileParser::ParseFile(result, fileHeader);

    if (error == kugou::KGMParseError::kUnsupportedFormat) {
      return false;
    }
  } while (error == kugou::KGMParseError::kNeedMoreData);

  if (error == kugou::KGMParseError::kSupportedKGM) {
    cipher_ = std::make_unique<kugou::KGMCipher>(result.file_key);
  } else if (error == kugou::KGMParseError::kSupportedVPR) {
    // TODO: remove this key
    std::array<uint8_t, 17> vpr_key = {
        0x25, 0xdf, 0xe8, 0xa6, 0x75, 0x1e, 0x75, 0x0e, 0x2f,
        0x80, 0xf3, 0x2d, 0xb8, 0xb6, 0xe3, 0x11, 0x00,
    };

    cipher_ = std::make_unique<kugou::VPRCipher>(result.file_key, vpr_key);
  } else {
    return false;
  }

  bof_bytes_ignore_ = result.bof_bytes_ignore;
  eof_bytes_ignore_ = 0;

  return true;
}

}  // namespace umd::utils::audio_decryptor
