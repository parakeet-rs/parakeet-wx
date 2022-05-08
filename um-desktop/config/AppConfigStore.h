#pragma once

#include "../types.h"

#include <filesystem>
#include <memory>

namespace umd::config {

struct GeneralConfig {
  int thread_count;
};

struct KugouConfig {
  Vec<u8> t1;
  Vec<u8> t2;
  Vec<u8> v2;

  Vec<u8> vpr_key;
};

struct XimalayaConfig {
  Vec<u8> x2m_content_key;

  Vec<u8> x3m_content_key;
  Vec<u8> x3m_scramble_indexes;
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
