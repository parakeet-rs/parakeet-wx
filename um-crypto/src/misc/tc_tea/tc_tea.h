#pragma once

#include "um-crypto/types.h"

// Tencent-TEA in cpp.

namespace umc::misc::tc_tea::ecb {

constexpr std::size_t BLOCK_SIZE = 8;

void DecryptBlock(void* block, u32* k);
void EncryptBlock(void* block, u32* k);

}  // namespace umc::misc::tc_tea::ecb

namespace umc::misc::tc_tea::cbc {

bool Decrypt(u8* plaindata, std::size_t& plaindata_len, const u8* cipher, std::size_t cipher_len, const u8* key);

std::size_t GetEncryptedSize(std::size_t size);

bool Encrypt(u8* cipher, std::size_t& cipher_len, const u8* plaintext, std::size_t plaintext_len, const u8* key);
}  // namespace umc::misc::tc_tea::cbc
