#pragma once
#include <cstdint>

// Tencent-TEA in cpp.

bool umc_tc_tea_cbc_decrypt(uint8_t* plaindata,
                            size_t& plaindata_len,
                            const uint8_t* cipher,
                            size_t cipher_len,
                            const uint8_t* key);
