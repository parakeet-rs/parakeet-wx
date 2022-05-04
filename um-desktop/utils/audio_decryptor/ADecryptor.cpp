#include "ADecryptor.h"

#include <filesystem>

namespace umd::utils::audio_decryptor {

void ADecryptor::Close() {
  if (in_file_.is_open()) {
    in_file_.close();
  }
}

void ADecryptor::Open(const std::filesystem::path& in_file_path) {
  in_file_path_ = in_file_path;
  in_file_.open(in_file_path, std::ifstream::binary | std::ifstream::in);

  // Get file size and seek to begining.
  in_file_.seekg(0, std::ifstream::end);
  file_size_ = in_file_.tellg();
  in_file_.seekg(0, std::ifstream::beg);
}

}  // namespace umd::utils::audio_decryptor
