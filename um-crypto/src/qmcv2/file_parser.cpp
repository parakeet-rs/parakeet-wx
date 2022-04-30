#include "../util/endian.h"
#include "../util/str_helper.h"
#include "um-crypto/qmcv2.h"

#include <cstdint>
#include <cstdlib>

const size_t kMinimalBytesForDetection = 20;

namespace umc::qmc::v2::parser {

inline const uint8_t* str_find_comma(const uint8_t* p, size_t len) {
  while (len-- > 0) {
    if (*p == ',') {
      return p;
    }

    p++;
  }
  return nullptr;
}

int parse_file_pc(PARSE_RESULT* result,
                  const uint8_t* eof_data,
                  size_t eof_len) {
  // Legacy PC QM encoded format.
  // ekey = [ansi ekey]
  // eof_mark = [uint32_t ekey_size]
  // file = [encrypted_data] [ekey] [eof_mark]
  if (eof_len < 4) {
    result->required_eof_size = 4;
    return umc::qmc::v2::parser::kParseErrorNeedMoreBytes;
  }

  auto ekey_size = *reinterpret_cast<uint32_t*>(eof_data[eof_len - 4]);

  // Currently known & supported largest key size, is 0x02C0 (704)
  if (ekey_size > 0x300) {
    return umc::qmc::v2::parser::kParseErrorUnsupported;
  }

  umc::str_from_ptr(result->ekey, eof_data + eof_len - 4 - ekey_size,
                    ekey_size);
  if (result->song_id) {
    umc::str_free(result->song_id);
  }

  return umc::qmc::v2::parser::kParseErrorOK;
}

int parse_file_qtag(PARSE_RESULT* result,
                    const uint8_t* eof_data,
                    size_t eof_len) {
  // Legacy Android format.
  // metadata = [ansi ekey] "," [ansi songid] "," [ansi metadata_version '2']
  // eof_mark = [uint32_t metadata_size] [uint32_t 'QTag']
  // file = [encrypted_data] [metadata] [eof_mark]

  if (eof_len < 8) {
    result->required_eof_size = 8;
    return umc::qmc::v2::parser::kParseErrorNeedMoreBytes;
  }

  auto metadata_size = *reinterpret_cast<uint32_t*>(eof_data[eof_len - 8]);
  metadata_size = umc_betoh_u32(metadata_size);

  size_t required_size = metadata_size + 8;
  if (required_size < eof_len) {
    result->required_eof_size = required_size;
    return umc::qmc::v2::parser::kParseErrorNeedMoreBytes;
  }

  const uint8_t* ekey_str = eof_data + eof_len - 8 - metadata_size;
  const uint8_t* songid_str = str_find_comma(ekey_str, metadata_size);
  if (songid_str++ == nullptr)
    return umc::qmc::v2::parser::kParseErrorUnknownFormat;
  const uint8_t* metadata_version = str_find_comma(songid_str, metadata_size);
  if (metadata_version++ == nullptr)
    return umc::qmc::v2::parser::kParseErrorUnknownFormat;
  if (*metadata_version != '2')
    return umc::qmc::v2::parser::kParseErrorUnknownFormat;

  result->required_eof_size = 0;

  umc::str_from_ptr(result->ekey, ekey_str, songid_str - ekey_str - 1);
  umc::str_from_ptr(result->song_id, songid_str,
                    metadata_version - songid_str - 1);

  return umc::qmc::v2::parser::kParseErrorOK;
}

PARSE_RESULT* result_new() {
  return static_cast<PARSE_RESULT*>(calloc(1, sizeof(PARSE_RESULT)));
}

void result_free(PARSE_RESULT* result) {
  assert(result);

  if (result->ekey)
    umc::str_free(result->ekey);

  if (result->song_id)
    umc::str_free(result->song_id);

  memset(result, 0, sizeof(result));
  free(result);
}

int parse_file(PARSE_RESULT* result, const uint8_t* eof_data, size_t eof_len) {
  if (eof_len < kMinimalBytesForDetection) {
    result->required_eof_size = kMinimalBytesForDetection;
    return umc::qmc::v2::parser::kParseErrorNeedMoreBytes;
  }

  // Check for QTag
  auto eof = *reinterpret_cast<uint32_t*>(eof_data[eof_len - 4]);

  if (eof == 'QTag') {
    return parse_file_qtag(result, eof_data, eof_len);
  } else if (eof == 'STag') {
    return umc::qmc::v2::parser::kParseErrorUnsupported;
  }

  return parse_file_pc(result, eof_data, eof_len);
}

}  // namespace umc::qmc::v2::parser
