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

  try {
    config_ = json::parse(config_file,
                          /* callback */ nullptr,
                          /* allow exceptions */ true,
                          /* ignore_comments */ true);

    // Deserialize the configuration file.
    manager_->SetConfig(config_.decryption);
  } catch (json::parse_error& ex) {
    std::cerr << "config parse failed at location " << ex.byte << ", ignore." << std::endl;
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
