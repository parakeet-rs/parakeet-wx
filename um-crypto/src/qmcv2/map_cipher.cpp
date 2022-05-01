#include "um-crypto/qmcv2/map_cipher.h"

using namespace umc;
using namespace umc::qmcv2;

inline u8 Rotate(u8 value, usize bits) {
  usize shift_amount = (bits + 4) % 8;
  auto left = value << shift_amount;
  auto right = value >> shift_amount;
  return u8(left | right);
}

inline u8 MapCipher::GetMaskByte() {
  if (offset > 0x7FFF)
    offset %= 0x7FFF;

  u64 idx = (offset * offset + 71214) % N;
  u8 value = key[idx];

  // Rotate by the lower 3 bits
  return Rotate(value, idx & 0b0111);
}

MapCipher::MapCipher(const Vec<u8>& key) : IStreamCipher() {
  this->key = key;
  this->N = key.size();
}

bool MapCipher::Encrypt(Vec<u8>& result, const Vec<u8>& input) {
  result.resize(input.size());

  const auto len = input.size();
  for (usize i = 0; i < len; i++) {
    result[i] = input[i] ^ GetMaskByte();
    offset++;
  }

  return true;
}

bool MapCipher::Decrypt(Vec<u8>& result, const Vec<u8>& input) {
  return Encrypt(result, input);
}
