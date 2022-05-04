#pragma once
#include "../common.h"

#include <boost/nowide/fstream.hpp>

#include <fstream>
#include <string>

namespace umd::utils::audio_decryptor {

enum class EncryptionType {
  kNotEncrypted = 0,
  kUnsupported = 1,
  kQQMusicV1 = 0x10,
  kQQMusicV2,
  kKugouMusic = 0x20,
  kXimalayaX2M = 0x30,
};

class ADecryptor {
 public:
  ADecryptor() {}
  virtual ~ADecryptor() { Close(); }
  virtual EncryptionType GetEncryptionType() const = 0;

  virtual void Open(const std::string& in_file_path);
  virtual void Close();

  /**
   * @brief Setup current decryptor.
   *
   * @param file_path
   * @return true  Input file is supported and can be decrypted.
   * @return false Input file is not supported by this decryptor.
   */
  virtual bool SetupDecryptor() = 0;

  /**
   * @brief Decrypt first block. For use with decrypted file type detection.
   *
   * @param buf
   * @param len
   * @return true
   * @return false
   */
  virtual bool DecryptFirstBlock(u8* buf, usize len = 40) = 0;
  virtual bool DecryptEntireFile(const std::string& out_path) = 0;

  inline const std::string& GetErrorMessage() { return error_msg_; }

 protected:
  boost::nowide::ifstream in_file_;
  usize file_size_;
  std::string in_file_path_;
  std::string error_msg_;

  // Inline helpers
  inline usize Read(u8* buf, usize len) {
    in_file_.read(reinterpret_cast<char*>(buf), len);
    return in_file_.gcount();
  }
};

}  // namespace umd::utils::audio_decryptor
