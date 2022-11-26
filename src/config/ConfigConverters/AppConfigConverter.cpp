#include "AppConfigConverter.h"
#include "KugouConfigConverter.h"

using json = nlohmann::json;

namespace parakeet_wx::config {

void to_json(json& j, const AppConfig& config) {
  j["general"] = config.desktop.general;
  j["kugou"] = config.decryption.kugou;
}

void from_json(const json& j, AppConfig& config) {
  AppConfig def;
  config.desktop.general = j.value("general", def.desktop.general);
  config.decryption.kugou = j.value("kugou", def.decryption.kugou);

  // TODO: Parse decryption configuration
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