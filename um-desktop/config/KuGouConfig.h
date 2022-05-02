#pragma once
#include "ConfigBase.h"

namespace umd::config {

enum class KuGouConfigItem {
  kTable1 = 0,
  kTable2,
  kTableV2,
  kTableVPR,
};

class KuGouConfig : public ConfigBase<KuGouConfigItem> {
  KuGouConfig() : ConfigBase() {
    SetBytesItem(KuGouConfigItem::kTable1, std::vector<uint8_t>());
    SetBytesItem(KuGouConfigItem::kTable2, std::vector<uint8_t>());
    SetBytesItem(KuGouConfigItem::kTableV2, std::vector<uint8_t>());
    SetBytesItem(KuGouConfigItem::kTableVPR, std::vector<uint8_t>());
  };
  virtual ~KuGouConfig();
};

};  // namespace umd::config
