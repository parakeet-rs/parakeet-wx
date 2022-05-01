#pragma once
#include <cstddef>
#include <cstdint>

#include "qmcv2/file_parser.h"
#include "qmcv2/key_derive.h"
#include "qmcv2/map_cipher.h"
#include "qmcv2/rc4_cipher.h"

namespace umc::qmc::v2 {

namespace parser {

const int kParseErrorOK = 0;
const int kParseErrorUnsupported = 1;
const int kParseErrorNeedMoreBytes = 2;
const int kParseErrorUnknownFormat = 3;

// QMC file parser
typedef struct PARSE_RESULT {
  size_t required_eof_size;
  char* ekey;
  char* song_id;
} PARSE_RESULT;

PARSE_RESULT* result_new();
void result_free(PARSE_RESULT* result);

int parse_file(PARSE_RESULT* result, const uint8_t* eof_data, size_t eof_len);

}  // namespace parser

}  // namespace umc::qmc::v2
