#pragma once
#include "ADecryptor.h"

#include <memory>

namespace umc {
class IStreamCipher;
}  // namespace umc

namespace umd::utils::audio_decryptor {

class AStreamDecryptor : public ADecryptor {
 public:
  AStreamDecryptor() {}
  virtual ~AStreamDecryptor() {}

  virtual bool DecryptFirstBlock(u8* buf, usize len = 40) override;
  virtual bool DecryptEntireFile(const std::string& out_path) override;

 protected:
  std::shared_ptr<umc::IStreamCipher> cipher_ = nullptr;
  usize bof_bytes_ignore_ = 0;
  usize eof_bytes_ignore_ = 0;
};

}  // namespace umd::utils::audio_decryptor
