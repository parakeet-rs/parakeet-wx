#include "AXorDecryptor.h"

#include "um-crypto/abstract/AXorStreamCipher.h"

#ifndef __UM_XOR_DECRYPT_BUFFER_LEN
#define __UM_XOR_DECRYPT_BUFFER_LEN (4 * 1024 * 1024)
#endif

namespace umd::utils::audio_decryptor {

const usize kXorDecryptBufferSize = __UM_XOR_DECRYPT_BUFFER_LEN;

bool AXorDecryptor::DecryptFirstBlock(u8* buf, usize len) {
  if (!cipher_ || !in_file_.is_open()) {
    return false;
  }

  // Fetch first block.
  Vec<u8> buf_in(len);
  cipher_->Seek(0);
  in_file_.seekg(bof_bytes_ignore_, std::ifstream::beg);
  auto bytes_read = Read(buf_in.data(), len);

  // Decrypt it.
  return cipher_->XorStream(buf, buf_in.data(), bytes_read);
}

bool AXorDecryptor::DecryptEntireFile(const std::string& out_path) {
  if (!cipher_ || !in_file_.is_open()) {
    return false;
  }

  usize bytes_left = file_size_ - bof_bytes_ignore_ - eof_bytes_ignore_;
  boost::nowide::ofstream out_file(out_path, std::ofstream::binary);
  Vec<u8> buf_encrypted(kXorDecryptBufferSize);
  Vec<u8> buf_decrypted(kXorDecryptBufferSize);

  // Reset seek
  cipher_->Seek(0);
  in_file_.seekg(bof_bytes_ignore_, std::ifstream::beg);

  while (bytes_left > 0) {
    const usize block_size = std::min(bytes_left, kXorDecryptBufferSize);

    // Read data
    const usize bytes_read = Read(buf_encrypted.data(), block_size);
    if (bytes_read == 0) {
      error_msg_ = _("Error: Unexpected EOF when reading input.");
      break;
    }

    cipher_->XorStream(buf_decrypted.data(), buf_encrypted.data(), bytes_read);
    out_file.write(reinterpret_cast<char*>(buf_decrypted.data()), bytes_read);
    bytes_left -= bytes_read;
  }

  in_file_.close();
  out_file.close();
  return bytes_left == 0;
}

}  // namespace umd::utils::audio_decryptor
