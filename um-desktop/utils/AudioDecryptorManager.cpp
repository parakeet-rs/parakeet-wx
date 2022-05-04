#include "AudioDecryptorManager.h"
#include "um-crypto/utils/DetectAudioType.h"

#include "audio_decryptor/KugouMusicDecryptor.h"
#include "audio_decryptor/QQMusicV1Decryptor.h"
#include "audio_decryptor/QQMusicV2Decryptor.h"
#include "audio_decryptor/XimalayaX2MDecryptor.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace umc;
using namespace umd;
using namespace umd::utils::audio_decryptor;

namespace umd::utils {

const usize kDecryptBufferSize = 4 * 1024 * 1024;

AudioDecryptorManager::AudioDecryptorManager() {
  Add(std::make_shared<QQMusicV1Decryptor>());
  Add(std::make_shared<QQMusicV2Decryptor>());
  Add(std::make_shared<KugouMusicDecryptor>());
  Add(std::make_shared<XimalayaX2MDecryptor>());
}

void AudioDecryptorManager::Open(const Path& file_path) {
  in_file_path_ = file_path;
  Close();
}

void AudioDecryptorManager::Close() {
  if (active_decryptor_) {
    active_decryptor_->Close();
    active_decryptor_ = nullptr;
  }
}

EncryptionType AudioDecryptorManager::SniffEncryption() {
  Close();

  // Check each decryptor we have registered.
  for (auto& decryptor : decryptors_) {
    decryptor->Open(in_file_path_);
    if (decryptor->SetupDecryptor()) {
      active_decryptor_ = decryptor;
      return decryptor->GetEncryptionType();
    }
    decryptor->Close();
  }

  return EncryptionType::kUnsupported;
}

bool AudioDecryptorManager::DecryptAudioFile() {
  if (!active_decryptor_) {
    return false;
  }

  Vec<u8> buf(umc::utils::kAudioTypeSniffBufferSize);
  if (!active_decryptor_->DecryptFirstBlock(buf.data(), buf.size())) {
    return false;
  }

  U8Str extension = umc::utils::DetectAudioExtensionU8(buf.data(), buf.size());
  std::filesystem::path in_file_path(in_file_path_);
  if (extension == in_file_path.extension()) {
    U8Str name = in_file_path.stem().u8string() + u8"_decoded" + extension;
    in_file_path.replace_filename(name);
  } else {
    in_file_path.replace_extension(extension);
  }
  return active_decryptor_->DecryptEntireFile(in_file_path.u8string());
}

}  // namespace umd::utils
