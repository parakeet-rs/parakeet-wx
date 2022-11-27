#pragma once

#include <parakeet-crypto/decryption/DecryptionManager.h>

#include <string>

namespace parakeet_wx::config {

struct GeneralConfig {
  int thread_count;
  std::string locale;

  GeneralConfig() : thread_count(4), locale(std::string("zh_CN")) {}
};

struct DesktopConfig {
  GeneralConfig general;
};

struct AppConfig {
  DesktopConfig desktop;
  parakeet_crypto::decryption::config::DecryptionConfig decryption;
};

}  // namespace parakeet_wx::config
