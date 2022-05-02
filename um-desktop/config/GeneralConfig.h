#pragma once
#include "ConfigBase.h"

namespace umd::config {

enum class GeneralConfigItem {
  kThreadCount = 0,
};

class GeneralConfig : public ConfigBase<GeneralConfigItem> {
  GeneralConfig() : ConfigBase() {
    SetIntItem(GeneralConfigItem::kThreadCount, -1);
  };
  virtual ~GeneralConfig();
};

};  // namespace umd::config
