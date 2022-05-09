#include "um-crypto/decryption/tencent/joox.h"
#include "um-crypto/endian.h"

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/sha.h>

#include <cassert>

namespace umc::decryption::tencent {

// Private implementation

namespace detail_joox_v4 {

constexpr usize kJooxMagicSize = 4;
constexpr usize kJooxVer4HeaderSize = 12; /* 'E!04' + u64_be(file size) */

constexpr u32 kMagicJooxV4 = 0x45'21'30'34;  // 'E!04'

// Input block + padding 16 bytes (of 0x10)
constexpr usize kJooxV4EncryptionBlockSize = 0x100000;
constexpr usize kJooxV4DecryptionBlockSize = kJooxV4EncryptionBlockSize + 0x10;

constexpr usize kJooxAESByteSize = 16;
constexpr usize kJooxKeyDeriveIter = 1000;

class JooxFileLoaderImpl : public JooxFileLoader {
 public:
  JooxFileLoaderImpl(const Str& install_uuid, const JooxSalt& salt) {
    u8 derived[CryptoPP::SHA1::DIGESTSIZE];
    CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA1> pbkdf;
    CryptoPP::byte unused = 0;
    pbkdf.DeriveKey(derived, sizeof(derived), 0 /* unused */,
                    reinterpret_cast<const u8*>(install_uuid.c_str()),
                    install_uuid.size(), salt.data(), salt.size(), 1000, 0);

    aes.SetKey(derived, kJooxAESByteSize);
  }

 private:
  CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption aes;

  bool Write(const u8* in, usize len) override {
    if (ReadUntilOffset(in, len, kJooxMagicSize)) {
      error_ = ReadBigEndian<u32>(buf_in_.data()) != kMagicJooxV4;
    }
    if (error_) return false;

    if (ReadUntilOffset(in, len, kJooxVer4HeaderSize)) {
      buf_in_.resize(0);
      buf_in_.reserve(kJooxV4DecryptionBlockSize);
    }

    while (ReadBlock(in, len, kJooxV4DecryptionBlockSize)) {
      if (!DecryptAesEcbPkcs5()) {
        return false;
      }
    }

    assert(len == 0);

    return true;
  }

  bool End() { return DecryptAesEcbPkcs5(); }

  bool DecryptAesEcbPkcs5() {
    const usize n = buf_in_.size();
    if (n % kJooxAESByteSize != 0) {
      error_ = true;
      return false;
    }

    u8* p_in = buf_in_.data();
    u8* end = p_in + n - kJooxAESByteSize;
    for (; p_in < end; p_in += kJooxAESByteSize) {
      aes.ProcessData(p_in, p_in, kJooxAESByteSize);
      buf_out_.insert(buf_out_.end(), p_in, p_in + kJooxAESByteSize);
    }

    aes.ProcessData(p_in, p_in, kJooxAESByteSize);

    // Trim padding
    usize len = kJooxAESByteSize - usize{p_in[kJooxAESByteSize - 1]};
    buf_out_.insert(buf_out_.end(), p_in, p_in + len);
    buf_in_.resize(0);

    return true;
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
