#pragma once

#include "KugouHeader.h"
#include "um-crypto/decryption/kugou/KugouFileLoader.h"

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

namespace umc::decryption::kugou {

struct KGMCryptoConfig {
  std::map<uint32_t, std::vector<uint8_t>> slot_keys;
  KugouV4SlotKeyExpansionTable v4_slot_key_expansion_table;
  KugouV4FileKeyExpansionTable v4_file_key_expansion_table;
};

inline uint8_t xor_u32_bytes(uint32_t x) {
  auto p = reinterpret_cast<const uint8_t*>(&x);
  return p[0] ^ p[1] ^ p[2] ^ p[3];
}

class KGMCrypto {
 public:
  virtual bool Configure(const KGMCryptoConfig& config,
                         const std::vector<uint8_t>& slot_key,
                         const kgm_file_header& header) = 0;
  virtual void Encrypt(uint64_t offset, uint8_t* buffer, size_t n) = 0;
  virtual void Decrypt(uint64_t offset, uint8_t* buffer, size_t n) = 0;
};

std::unique_ptr<KGMCrypto> create_kugou_decryptor(
    const kgm_file_header& header,
    const KGMCryptoConfig& config);

// Copy-paste of decryptor definitions...

class KGMCrypto2 : public KGMCrypto {
 public:
  virtual bool Configure(const KGMCryptoConfig& config,
                         const std::vector<uint8_t>& slot_key,
                         const kgm_file_header& header) override;
  virtual void Encrypt(uint64_t offset, uint8_t* buffer, size_t n) override;
  virtual void Decrypt(uint64_t offset, uint8_t* buffer, size_t n) override;

 private:
  std::vector<uint8_t> key_;
};

class KGMCrypto3 : public KGMCrypto {
 public:
  virtual bool Configure(const KGMCryptoConfig& config,
                         const std::vector<uint8_t>& slot_key,
                         const kgm_file_header& header) override;
  virtual void Encrypt(uint64_t offset, uint8_t* buffer, size_t n) override;
  virtual void Decrypt(uint64_t offset, uint8_t* buffer, size_t n) override;

 private:
  std::array<uint8_t, 16> slot_key_;
  std::array<uint8_t, 17> file_key_;
};

class KGMCrypto4 : public KGMCrypto {
 public:
  virtual bool Configure(const KGMCryptoConfig& config,
                         const std::vector<uint8_t>& slot_key,
                         const kgm_file_header& header) override;
  virtual void Encrypt(uint64_t offset, uint8_t* buffer, size_t n) override;
  virtual void Decrypt(uint64_t offset, uint8_t* buffer, size_t n) override;

 private:
  std::vector<uint8_t> slot_key_;
  std::vector<uint8_t> file_key_;
};

}  // namespace umc::decryption::kugou
