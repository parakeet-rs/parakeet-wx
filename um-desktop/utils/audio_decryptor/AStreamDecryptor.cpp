#include "AStreamDecryptor.h"

#include "um-crypto/abstract/AXorStreamCipher.h"

#ifndef __UM_DECRYPT_BUFFER_LEN
#define __UM_DECRYPT_BUFFER_LEN (4 * 1024 * 1024)
#endif

namespace umd::utils::audio_decryptor {

const usize kDecryptionBufferLen = __UM_DECRYPT_BUFFER_LEN;
const usize kEncryptionBufferLen = kDecryptionBufferLen + 1024;

bool AStreamDecryptor::DecryptFirstBlock(u8* buf, usize len) {
  if (!cipher_ || !in_file_.is_open()) {
    return false;
  }

  // Fetch first block.
  Vec<u8> buf_in(len);
  cipher_->Seek(0);
  in_file_.seekg(bof_bytes_ignore_, std::ifstream::beg);
  auto bytes_read = Read(buf_in.data(), len);

  // Decrypt it.
  return cipher_->Decrypt(buf, len, buf_in.data(), bytes_read);
}

bool AStreamDecryptor::DecryptEntireFile(const Path& out_path) {
  if (!cipher_ || !in_file_.is_open()) {
    return false;
  }

  usize bytes_left = file_size_ - bof_bytes_ignore_ - eof_bytes_ignore_;
  std::ofstream out_file(out_path, std::ofstream::binary);
  Vec<u8> buf_encrypted(kEncryptionBufferLen);
  Vec<u8> buf_decrypted(kDecryptionBufferLen);

  u8* p_out = buf_encrypted.data();
  u8* p_in = buf_decrypted.data();

  // Reset seek
  cipher_->Seek(0);
  in_file_.seekg(bof_bytes_ignore_, std::ifstream::beg);

  while (bytes_left > 0) {
    const usize block_size = std::min(bytes_left, kDecryptionBufferLen);

    // Read data
    const usize bytes_read = Read(buf_encrypted.data(), block_size);
    if (bytes_read == 0) {
      error_msg_ = _("Error: Unexpected EOF when reading input.");
      break;
    }

    usize bytes_decrypted = kDecryptionBufferLen;
    if (!cipher_->Decrypt(p_in, bytes_decrypted, p_out, bytes_read)) {
      error_msg_ = _("Error: cipher reported failure.");
      break;
    }
    out_file.write(reinterpret_cast<char*>(p_in), bytes_decrypted);
    bytes_left -= bytes_read;
  }

  in_file_.close();
  out_file.close();
  return bytes_left == 0;
}

}  // namespace umd::utils::audio_decryptor
