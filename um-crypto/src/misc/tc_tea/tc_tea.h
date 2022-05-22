#pragma once

#include "um-crypto/types.h"

// Tencent-TEA in cpp.

namespace umc::misc::tc_tea::ecb {

constexpr usize BLOCK_SIZE = 8;

void DecryptBlock(void* block, u32* k);
void EncryptBlock(void* block, u32* k);

}  // namespace umc::misc::tc_tea::ecb

namespace umc::misc::tc_tea::cbc {

bool Decrypt(u8* plaindata,
             usize& plaindata_len,
             const u8* cipher,
             usize cipher_len,
             const u8* key);

usize GetEncryptedSize(usize size);

bool Encrypt(u8* cipher,
             usize& cipher_len,
             const u8* plaintext,
             usize plaintext_len,
             const u8* key);
}  // namespace umc::misc::tc_tea::cbc
