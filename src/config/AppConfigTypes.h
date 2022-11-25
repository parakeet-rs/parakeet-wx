#pragma once

#include <parakeet-crypto/decryption/DecryptionManager.h>

#include <string>

namespace parakeet_wx::config {

struct GeneralConfig {
  int thread_count;
  std::string locale;
};

struct DesktopConfig {
  GeneralConfig general;
};

struct AppConfig {
  DesktopConfig desktop;
  parakeet_crypto::decryption::config::DecryptionConfig decryption;
};

}  // namespace parakeet_wx::config
