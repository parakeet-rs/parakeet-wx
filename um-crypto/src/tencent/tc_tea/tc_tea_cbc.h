#pragma once
#include <cstddef>
#include <cstdint>

// Tencent-TEA in cpp.

namespace umc::tc_tea {

bool cbc_decrypt(uint8_t* plaindata,
                 size_t& plaindata_len,
                 const uint8_t* cipher,
                 size_t cipher_len,
                 const uint8_t* key);

}  // namespace umc::tc_tea
