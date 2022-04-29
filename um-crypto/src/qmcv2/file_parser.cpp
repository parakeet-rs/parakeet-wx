#include "../util/endian.h"
#include "../util/str_helper.h"
#include "um-crypto/qmcv2.h"

#include <cstdint>
#include <cstdlib>

const size_t kMinimalBytesForDetection = 20;

int umc_qmcv2_parse_file_pc(umc_qmc_parse_result* result,
                            const uint8_t* eof_data,
                            size_t eof_len) {
  // Legacy PC QM encoded format.
  // ekey = [ansi ekey]
  // eof_mark = [uint32_t ekey_size]
  // file = [encrypted_data] [ekey] [eof_mark]
  if (eof_len < 4) {
    result->required_eof_size = 4;
    return kQMCParseErrorNeedMoreBytes;
  }

  auto ekey_size = *reinterpret_cast<uint32_t*>(eof_data[eof_len - 4]);

  // Currently known & supported largest key size, is 0x02C0 (704)
  if (ekey_size > 0x300) {
    return kQMCParseErrorUnsupported;
  }

  umc_str_from_ptr(result->ekey, eof_data + eof_len - 4 - ekey_size, ekey_size);
  if (result->song_id) {
    umc_str_free(result->song_id);
  }

  return kQMCParseErrorOK;
}

const uint8_t* str_find_comma(const uint8_t* p, size_t len) {
  while (len-- > 0) {
    if (*p == ',') {
      return p;
    }

    p++;
  }
  return nullptr;
}

int umc_qmcv2_parse_file_qtag(umc_qmc_parse_result* result,
                              const uint8_t* eof_data,
                              size_t eof_len) {
  // Legacy Android format.
  // metadata = [ansi ekey] "," [ansi songid] "," [ansi metadata_version '2']
  // eof_mark = [uint32_t metadata_size] [uint32_t 'QTag']
  // file = [encrypted_data] [metadata] [eof_mark]

  if (eof_len < 8) {
    result->required_eof_size = 8;
    return kQMCParseErrorNeedMoreBytes;
  }

  auto metadata_size = *reinterpret_cast<uint32_t*>(eof_data[eof_len - 8]);
  metadata_size = umc_betoh_u32(metadata_size);

  size_t required_size = metadata_size + 8;
  if (required_size < eof_len) {
    result->required_eof_size = required_size;
    return kQMCParseErrorNeedMoreBytes;
  }

  const uint8_t* ekey_str = eof_data + eof_len - 8 - metadata_size;
  const uint8_t* songid_str = str_find_comma(ekey_str, metadata_size);
  if (songid_str++ == nullptr)
    return kQMCParseErrorUnknownFormat;
  const uint8_t* metadata_version = str_find_comma(songid_str, metadata_size);
  if (metadata_version++ == nullptr)
    return kQMCParseErrorUnknownFormat;
  if (*metadata_version != '2')
    return kQMCParseErrorUnknownFormat;

  result->required_eof_size = 0;

  umc_str_from_ptr(result->ekey, ekey_str, songid_str - ekey_str - 1);
  umc_str_from_ptr(result->song_id, songid_str,
                   metadata_version - songid_str - 1);

  return kQMCParseErrorOK;
}

umc_qmc_parse_result* umc_qmc_parse_result_new() {
  return static_cast<umc_qmc_parse_result*>(
      calloc(1, sizeof(umc_qmc_parse_result)));
}

void umc_qmc_parse_result_free(umc_qmc_parse_result* result) {
  assert(result);

  if (result->ekey)
    umc_str_free(result->ekey);

  if (result->song_id)
    umc_str_free(result->song_id);

  memset(result, 0, sizeof(umc_qmc_parse_result));
  free(result);
}

int umc_qmcv2_parse_file(umc_qmc_parse_result* result,
                         const uint8_t* eof_data,
                         size_t eof_len) {
  if (eof_len < kMinimalBytesForDetection) {
    result->required_eof_size = kMinimalBytesForDetection;
    return kQMCParseErrorNeedMoreBytes;
  }

  // Check for QTag
  auto eof = *reinterpret_cast<uint32_t*>(eof_data[eof_len - 4]);

  if (eof == 'QTag') {
    return umc_qmcv2_parse_file_qtag(result, eof_data, eof_len);
  } else if (eof == 'STag') {
    return kQMCParseErrorUnsupported;
  }

  return umc_qmcv2_parse_file_pc(result, eof_data, eof_len);
}
