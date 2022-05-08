#pragma once

#include "../types.h"

#include <filesystem>
#include <memory>

#include <um-crypto.h>

namespace umd::config {

struct GeneralConfig {
  int thread_count;
};

struct KugouConfig {
  umc::kugou::KugouTable t1;
  umc::kugou::KugouTable t2;
  umc::kugou::KugouTable v2;

  umc::kugou::KugouFileKey vpr_key;
};

struct XimalayaConfig {
  umc::ximalaya::XimalayaX2MContentKey x2m_content_key;

  umc::ximalaya::XimalayaHeaderContentKey x3m_content_key;
  umc::ximalaya::XimalayaHeaderScrambleTable x3m_scramble_indexes;
};

struct AppConfig {
  GeneralConfig general;
  KugouConfig kugou;
  XimalayaConfig xmly;
};

class AppConfigStore {
 public:
  static inline AppConfigStore* GetInstance() { return instance_; }

  AppConfigStore();
  virtual ~AppConfigStore(){};

  bool LoadConfigFromDisk();
  bool SaveConfigToDisk();

  inline const AppConfig& GetLoadedConfig() const { return config_; }
  inline void UpdateConfig(const AppConfig& config) { config_ = config; }

 private:
  AppConfig config_ = {};
  std::filesystem::path config_file_path_;
  static AppConfigStore* instance_;
};

}  // namespace umd::config
