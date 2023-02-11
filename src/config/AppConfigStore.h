#pragma once

#include "AppConfigTypes.h"
#include "decryption_manager/decryption_manager.h"

#include <filesystem>
#include <memory>
namespace parakeet_wx::config
{

class AppConfigStore
{
  public:
    static inline AppConfigStore *GetInstance()
    {
        return instance_;
    }

    AppConfigStore();
    virtual ~AppConfigStore(){};

    bool LoadConfigFromDisk();
    bool SaveConfigToDisk();

    inline const AppConfig &GetLoadedConfig() const
    {
        return config_;
    }
    inline void UpdateConfig(AppConfig config)
    {
        config_ = std::move(config);
        manager_->SetConfig(config_);
    }
    inline std::shared_ptr<decryption_manager::DecryptionManager> GetDecryptionManager() const
    {
        return manager_;
    }

  private:
    AppConfig config_ = {};
    std::filesystem::path config_file_path_;
    std::shared_ptr<decryption_manager::DecryptionManager> manager_;

    static AppConfigStore *instance_;
};

} // namespace parakeet_wx::config
