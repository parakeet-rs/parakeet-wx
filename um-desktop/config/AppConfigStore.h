#pragma once

#include "../types.h"

#include <filesystem>
#include <memory>

#include <um-crypto/decryption/DecryptionManager.h>

namespace umd::config {

struct GeneralConfig {
  int thread_count;
};

struct DesktopConfig {
  GeneralConfig general;
};

struct AppConfig {
  DesktopConfig desktop;
  umc::decryption::config::DecryptionConfig decryption;
};

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
  inline std::shared_ptr<umc::decryption::DecryptionManager>
  GetDecryptionManager() const {
    return manager_;
  }

 private:
  AppConfig config_ = {};
  std::filesystem::path config_file_path_;
  std::shared_ptr<umc::decryption::DecryptionManager> manager_;

  static AppConfigStore* instance_;
};

}  // namespace umd::config
