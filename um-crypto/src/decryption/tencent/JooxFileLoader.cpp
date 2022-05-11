#include "um-crypto/decryption/tencent/JooxFileLoader.h"
#include "um-crypto/endian.h"

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>

#include <cassert>

namespace umc::decryption::tencent {

// Private implementation

namespace detail_joox_v4 {

constexpr usize kMagicSize = 4;
constexpr usize kVer4HeaderSize = 12; /* 'E!04' + u64_be(file size) */

constexpr u32 kMagicJooxV4 = 0x45'21'30'34;  // 'E!04'

// Input block + padding 16 bytes (of 0x10)
constexpr usize kAESBlockSize = 0x10;
constexpr usize kEncryptionBlockSize = 0x100000;
constexpr usize kDecryptionBlockSize = kEncryptionBlockSize + 0x10;
constexpr usize kBlockCountPerIteration = kEncryptionBlockSize / kAESBlockSize;

enum class State {
  kWaitForHeader = 0,
  kSeekToBody,
  kFastFirstPageDecryption,
  kDecryptPaddingBlock,
};

class JooxFileLoaderImpl : public JooxFileLoader {
 public:
  JooxFileLoaderImpl(const Str& install_uuid, const JooxSalt& salt) {
    u8 derived[CryptoPP::SHA1::DIGESTSIZE];
    CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA1> pbkdf;
    CryptoPP::byte unused = 0;
    pbkdf.DeriveKey(derived, sizeof(derived), 0 /* unused */,
                    reinterpret_cast<const u8*>(install_uuid.c_str()),
                    install_uuid.size(), salt.data(), salt.size(), 1000, 0);

    aes.SetKey(derived, kAESBlockSize);
  }

 private:
  CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption aes;

  State state_ = State::kWaitForHeader;
  usize block_count_ = 0;
  bool Write(const u8* in, usize len) override {
    buf_out_.reserve(buf_out_.size() + len);

    while (len) {
      switch (state_) {
        case State::kWaitForHeader:
          if (ReadUntilOffset(in, len, kMagicSize)) {
            error_ = ReadBigEndian<u32>(buf_in_.data()) != kMagicJooxV4;
            if (error_) return false;
            state_ = State::kSeekToBody;
          }
          break;
        case State::kSeekToBody:
          if (ReadUntilOffset(in, len, kVer4HeaderSize)) {
            buf_in_.erase(buf_in_.begin(), buf_in_.begin() + kVer4HeaderSize);
            state_ = State::kFastFirstPageDecryption;
          }
          break;
        case State::kFastFirstPageDecryption:
          // Always reserve last 16 bytes, as it could be the padding.
          while (ReadBlock(in, len, kAESBlockSize * 2)) {
            if (!DecryptAesBlock()) {
              error_ = true;
              return false;
            }
            block_count_++;
            if (block_count_ == kBlockCountPerIteration) {
              state_ = State::kDecryptPaddingBlock;
              break;
            }
          }
          break;
        case State::kDecryptPaddingBlock:
          if (ReadBlock(in, len, kAESBlockSize)) {
            if (!DecryptPaddingBlock()) {
              error_ = true;
              return false;
            }
            state_ = State::kFastFirstPageDecryption;
            block_count_ = 0;
          }
          break;
      }
    }

    return true;
  }

  inline bool DecryptAesBlock() {
    auto pos = buf_out_.size();
    buf_out_.resize(pos + kAESBlockSize);

    aes.ProcessData(&buf_out_[pos], buf_in_.data(), kAESBlockSize);

    offset_ += kAESBlockSize;
    buf_in_.erase(buf_in_.begin(), buf_in_.begin() + kAESBlockSize);
    return true;
  }

  inline bool DecryptPaddingBlock() {
    u8 block[kAESBlockSize];
    aes.ProcessData(block, buf_in_.data(), kAESBlockSize);

    // Trim data
    usize len = kAESBlockSize - block[kAESBlockSize - 1];
    if (len) {
      buf_out_.insert(buf_out_.end(), block, block + len);
    }

    offset_ += kAESBlockSize;
    buf_in_.erase(buf_in_.begin(), buf_in_.begin() + kAESBlockSize);
    return true;
  }

  bool End() override {
    if (error_) return false;
    if (buf_in_.size() == 0) return true;

    if (buf_in_.size() != 16) {
      error_ = true;
      return false;
    }

    auto ok = DecryptPaddingBlock();
    error_ = !ok;
    return ok;
  }
};

}  // namespace detail_joox_v4

// Public interface

std::unique_ptr<JooxFileLoader> JooxFileLoader::Create(const Str& install_uuid,
                                                       const JooxSalt& salt) {
  return std::make_unique<detail_joox_v4::JooxFileLoaderImpl>(install_uuid,
                                                              salt);
}

}  // namespace umc::decryption::tencent
