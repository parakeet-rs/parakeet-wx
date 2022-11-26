#include "AppConfigStore.h"
#include "ConfigConverters/AppConfigConverter.h"
#include "utils/AppDataPath.h"

#include <parakeet-crypto/utils/base64.h>

#include <fstream>
#include <iomanip>

namespace parakeet_wx::config {

AppConfigStore* AppConfigStore::instance_ = new AppConfigStore();

AppConfigStore::AppConfigStore() {
  manager_ = parakeet_crypto::decryption::DecryptionManager::Create();
  config_file_path_ = parakeet_wx::utils::GetUserDataDirectory() / "config.json5";
}

bool AppConfigStore::LoadConfigFromDisk() {
  std::ifstream config_file(config_file_path_, std::ios::in | std::ios::binary);
  if (config_file.fail()) {
    return false;
  }

  auto j = json::parse(config_file,
                       /* callback */ nullptr,
                       /* allow exceptions */ false,
                       /* ignore_comments */ true);
  if (j.is_discarded()) {
    std::cerr << "config parse failed, ignore config." << std::endl;
  } else {
    config_ = j.get<AppConfig>();
  }

  return true;
}

bool AppConfigStore::SaveConfigToDisk() {
  std::ofstream config_file(config_file_path_, std::ios::out | std::ios::binary);

  json configJson = config_;
  config_file << std::setw(2) << configJson;

  return true;
}

}  // namespace parakeet_wx::config
