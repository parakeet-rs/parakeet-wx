#include "AppConfigConverter.h"
#include "utils/json/generated/JooxConfigConverter.h"
#include "utils/json/generated/KugouConfigConverter.h"
#include "utils/json/generated/KuwoConfigConverter.h"
#include "utils/json/generated/NeteaseConfigConverter.h"
#include "utils/json/generated/QMCConfigConverter.h"
#include "utils/json/generated/XimalayaConfigConverter.h"

using json = nlohmann::json;

namespace parakeet_wx::config {

void to_json(json& j, const AppConfig& config) {
  j["general"] = config.desktop.general;

  j["kugou"] = config.decryption.kugou;
  j["kuwo"] = config.decryption.kuwo;
  j["netease"] = config.decryption.netease;
  j["qmc"] = config.decryption.qmc;
  j["joox"] = config.decryption.joox;
  j["ximalaya"] = config.decryption.ximalaya;
}

void from_json(const json& j, AppConfig& config) {
  AppConfig def;
  config.desktop.general = j.value("general", def.desktop.general);

  config.decryption.kugou = j.value("kugou", def.decryption.kugou);
  config.decryption.kuwo = j.value("kuwo", def.decryption.kuwo);
  config.decryption.netease = j.value("netease", def.decryption.netease);
  config.decryption.qmc = j.value("qmc", def.decryption.qmc);
  config.decryption.joox = j.value("joox", def.decryption.joox);
  config.decryption.ximalaya = j.value("ximalaya", def.decryption.ximalaya);
}

void to_json(json& j, const GeneralConfig& config) {
  j["locale"] = config.locale;
  j["thread_count"] = config.thread_count;
}

void from_json(const json& j, GeneralConfig& config) {
  GeneralConfig def;
  config.locale = j.value("locale", def.locale);
  config.thread_count = j.value("thread_count", def.thread_count);
}

}  // namespace parakeet_wx::config
