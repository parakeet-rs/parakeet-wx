#pragma once
#include <um-crypto/interface/IStreamCipher.h>
#include "ADecryptor.h"

#include <memory>

namespace umd::utils::audio_decryptor {

class AStreamDecryptor : public ADecryptor {
 public:
  AStreamDecryptor() {}
  virtual ~AStreamDecryptor() {}

  virtual bool DecryptFirstBlock(u8* buf, usize len = 40) override;
  virtual bool DecryptEntireFile(const Path& out_path) override;

 protected:
  std::unique_ptr<umc::IStreamCipher> cipher_ = nullptr;
  usize bof_bytes_ignore_ = 0;
  usize eof_bytes_ignore_ = 0;
};

}  // namespace umd::utils::audio_decryptor
