#pragma once
#include <cstdint>

// Map encryption.
typedef struct qmcv2_map qmcv2_map;
qmcv2_map* qmcv2_map_new(const uint8_t* key, size_t key_size);
uint8_t qmcv2_map_get_key_at_offset(qmcv2_map* ctx, size_t offset);
void qmcv2_map_encrypt(qmcv2_map* ctx, uint8_t* buf, size_t len, size_t offset);
void qmcv2_map_decrypt(qmcv2_map* ctx, uint8_t* buf, size_t len, size_t offset);
void qmcv2_map_free(qmcv2_map*& ctx);

// RC4 encryption.
typedef struct qmcv2_rc4 qmcv2_rc4;
qmcv2_rc4* qmcv2_rc4_new(const uint8_t* key, size_t key_size);
void qmcv2_rc4_encrypt(qmcv2_rc4* ctx, uint8_t* buf, size_t len, size_t offset);
void qmcv2_rc4_decrypt(qmcv2_rc4* ctx, uint8_t* buf, size_t len, size_t offset);
void qmcv2_rc4_free(qmcv2_rc4*& ctx);

// Key derive

void umc_qmcv2_free_derived_key(uint8_t*& key);
bool umc_qmcv2_derive_from_ekey(uint8_t*& derived_key,
                                size_t& derived_key_len,
                                const uint8_t* ekey,
                                size_t ekey_len);
bool umc_qmcv2_derive_from_ekey_b64(uint8_t*& derived_key,
                                    size_t& derived_key_len,
                                    const char* ekey_b64);

const int kQMCParseErrorOK = 0;
const int kQMCParseErrorUnsupported = 1;
const int kQMCParseErrorNeedMoreBytes = 2;
const int kQMCParseErrorUnknownFormat = 3;

// QMC file parser
typedef struct umc_qmc_parse_result {
  size_t required_eof_size;
  char* ekey;
  char* song_id;
} umc_qmc_parse_result;

int umc_qmcv2_parse_file(umc_qmc_parse_result* result,
                         const uint8_t* eof_data,
                         size_t eof_len);
