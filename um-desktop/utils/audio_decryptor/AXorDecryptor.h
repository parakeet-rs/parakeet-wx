#pragma once
#include "ADecryptor.h"

#include <memory>

namespace umc {
class AXorStreamCipher;
}  // namespace umc

namespace umd::utils::audio_decryptor {

class AXorDecryptor : public ADecryptor {
 public:
  AXorDecryptor() {}
  virtual ~AXorDecryptor() {}

  virtual bool DecryptFirstBlock(u8* buf, usize len = 40) override;
  virtual bool DecryptEntireFile(const std::string& out_path) override;

 protected:
  std::shared_ptr<umc::AXorStreamCipher> cipher_ = nullptr;
  usize bof_bytes_ignore_ = 0;
  usize eof_bytes_ignore_ = 0;
};

}  // namespace umd::utils::audio_decryptor
