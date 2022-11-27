#pragma once

#include "AppConfigTypes.h"

#include <filesystem>
#include <memory>
namespace parakeet_wx::config {

class AppConfigStore {
 public:
  static inline AppConfigStore* GetInstance() { return instance_; }

  AppConfigStore();
  virtual ~AppConfigStore(){};

  bool LoadConfigFromDisk();
  bool SaveConfigToDisk();

  inline const AppConfig& GetLoadedConfig() const { return config_; }
  inline void UpdateConfig(const AppConfig& config) {
    config_ = config;
    manager_->SetConfig(config_.decryption);
  }
  inline std::shared_ptr<parakeet_crypto::decryption::DecryptionManager> GetDecryptionManager() const {
    return manager_;
  }

 private:
  AppConfig config_ = {};
  std::filesystem::path config_file_path_;
  std::shared_ptr<parakeet_crypto::decryption::DecryptionManager> manager_;

  static AppConfigStore* instance_;
};

}  // namespace parakeet_wx::config
