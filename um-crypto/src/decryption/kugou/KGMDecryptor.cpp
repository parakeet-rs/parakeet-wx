#include "KGMDecryptor.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <span>

namespace umc::decryption::kugou {

inline std::unique_ptr<KGMCrypto> create_kgm_crypto(const kgm_file_header& header, const KGMCryptoConfig& config) {
  auto slot_key_it = config.slot_keys.find(header.key_slot);
  if (slot_key_it == config.slot_keys.end()) {
    return nullptr;
  }

  std::unique_ptr<KGMCrypto> kgm_crypto;

  switch (header.encryption_type) {
    case 2:
      kgm_crypto = std::make_unique<KGMCrypto2>();
      break;
    case 3:
      kgm_crypto = std::make_unique<KGMCrypto3>();
      break;
    case 4:
      kgm_crypto = std::make_unique<KGMCrypto4>();
      break;

    default:
      return nullptr;
  }

  if (!kgm_crypto->Configure(config, slot_key_it->second, header)) {
    return nullptr;
  }

  return kgm_crypto;
}

const std::array<uint8_t, 16> kKGMFileMagic = {
    0x7c, 0xd5, 0x32, 0xeb, 0x86, 0x02, 0x7f, 0x4b, 0xa8, 0xaf, 0xa6, 0x8e, 0x0f, 0xff, 0x99, 0x14,
};

const std::array<uint8_t, 16> kKGMChallengeBytes = {
    0x38, 0x85, 0xED, 0x92, 0x79, 0x5F, 0xF8, 0x4C, 0xB3, 0x03, 0x61, 0x41, 0x16, 0xA0, 0x1D, 0x47,
};

const std::array<uint8_t, 16> kVPRFileMagic = {
    0x05, 0x28, 0xbc, 0x96, 0xe9, 0xe4, 0x5a, 0x43, 0x91, 0xaa, 0xbd, 0xd0, 0x7a, 0xf5, 0x36, 0x31,
};

const std::array<uint8_t, 16> kVPRChallengeBytes = {
    0x1D, 0x5A, 0x05, 0x34, 0x0C, 0x41, 0x8D, 0x42, 0x9C, 0x83, 0x92, 0x6C, 0xAE, 0x16, 0xFE, 0x56,
};

enum class KGMType { kUnknown = 0, kKGM, kVPR };

std::unique_ptr<KGMCrypto> create_kugou_decryptor(const kgm_file_header& header, const KGMCryptoConfig& config) {
  KGMType kgm_type = KGMType::kUnknown;
  if (std::equal(kKGMFileMagic.begin(), kKGMFileMagic.end(), header.magic)) {
    kgm_type = KGMType::kKGM;
  } else if (std::equal(kVPRFileMagic.begin(), kVPRFileMagic.end(), header.magic)) {
    kgm_type = KGMType::kVPR;
  } else {
    return nullptr;
  }

  auto kgm_crypto = create_kgm_crypto(header, config);
  if (!kgm_crypto) {
    return nullptr;
  }

  // Validate the file key.
  std::span challenge(&header.key_challenge[0], sizeof(header.key_challenge));
  std::array<uint8_t, sizeof(header.key_challenge)> challenge_buffer;
  std::copy(challenge.begin(), challenge.end(), challenge_buffer.begin());
  kgm_crypto->Decrypt(0, challenge_buffer.data(), sizeof(challenge_buffer));

  auto challenge_expected = kgm_type == KGMType::kKGM ? kKGMChallengeBytes : kVPRChallengeBytes;

  if (!std::equal(challenge_buffer.begin(), challenge_buffer.end(), challenge_expected.begin())) {
    return nullptr;
  }

  return kgm_crypto;
}

}  // namespace umc::decryption::kugou
