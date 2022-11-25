#include "AppConfigStore.h"
#include "AppConfigConverter.h"
#include "utils/AppDataPath.h"

#include <parakeet-crypto/utils/base64.h>

#include <fstream>

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

  config_file.seekg(0, std::ios::end);
  std::size_t config_file_size = config_file.tellg();

  if (config_file_size > 0) {
    std::string config_json_str;
    config_json_str.resize(config_file_size);

    config_file.seekg(0, std::ios::beg);
    config_file.read(config_json_str.data(), config_json_str.size());
    config_file.close();

    try {
      config_ = json::parse(config_json_str);
    } catch (json::parse_error& ex) {
      std::cerr << "config parse failed at location " << ex.byte << ", ignore." << std::endl;
    }

    // Deserialize the configuration file.
    manager_->SetConfig(config_.decryption);
  }

  return true;
}

bool AppConfigStore::SaveConfigToDisk() {
  std::ofstream config_file(config_file_path_, std::ios::out | std::ios::binary);

  // Serialize the configuration file.
  json configJson = config_;
  auto config_json_str = configJson.dump(2);
  config_file.write(config_json_str.c_str(), config_json_str.size());
  config_file.close();

  return true;
}

}  // namespace parakeet_wx::config
