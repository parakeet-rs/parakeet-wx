#pragma once

#include <cstddef>
#include <cstdint>

// Tencent-TEA in cpp.

namespace umc::misc::tc_tea::ecb {

constexpr std::size_t BLOCK_SIZE = 8;

void DecryptBlock(void* block, uint32_t* k);
void EncryptBlock(void* block, uint32_t* k);

}  // namespace umc::misc::tc_tea::ecb

namespace umc::misc::tc_tea::cbc {

bool Decrypt(uint8_t* plaindata,
             std::size_t& plaindata_len,
             const uint8_t* cipher,
             std::size_t cipher_len,
             const uint8_t* key);

std::size_t GetEncryptedSize(std::size_t size);

bool Encrypt(uint8_t* cipher,
             std::size_t& cipher_len,
             const uint8_t* plaintext,
             std::size_t plaintext_len,
             const uint8_t* key);
}  // namespace umc::misc::tc_tea::cbc
