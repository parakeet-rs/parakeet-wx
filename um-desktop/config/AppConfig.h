#pragma once
#include <wx/fileconf.h>
#include <memory>

#include "GeneralConfig.h"
#include "KuGouConfig.h"

namespace umd::config {

class AppConfig {
 public:
  AppConfig();
  virtual ~AppConfig() {}

  std::shared_ptr<KuGouConfig> kugou;
  std::shared_ptr<GeneralConfig> general;
};

}  // namespace umd::config
