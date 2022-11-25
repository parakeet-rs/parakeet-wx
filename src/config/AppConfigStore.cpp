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
  std::string configJsonStr;
  std::ifstream config_file(config_file_path_, std::fstream::binary);
  config_file.seekg(0, std::ios::end);
  configJsonStr.resize(config_file.tellg());

  config_file.seekg(0, std::ios::beg);
  config_file.read(configJsonStr.data(), configJsonStr.size());
  config_file.close();

  config_ = json::parse(configJsonStr);

  // Deserialize the configuration file.
  manager_->SetConfig(config_.decryption);

  return true;
}

bool AppConfigStore::SaveConfigToDisk() {
  std::ofstream config_file(config_file_path_, std::fstream::out | std::fstream::binary);

  // Serialize the configuration file.
  json configJson = config_;
  auto configJsonStr = configJson.dump(2);
  config_file.write(configJsonStr.c_str(), configJsonStr.size());
  config_file.close();

  return true;
}

}  // namespace parakeet_wx::config
