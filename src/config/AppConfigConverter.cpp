#include "AppConfigConverter.h"

using json = nlohmann::json;

namespace parakeet_wx::config {

void to_json(json& j, const AppConfig& config) {
  j["general"] = config.desktop.general;
}

void from_json(const json& j, AppConfig& config) {
  config.desktop.general = j["general"];

  // TODO: Parse decryption configuration
}

void to_json(json& j, const GeneralConfig& config) {
  j["locale"] = config.locale;
  j["thread_count"] = config.thread_count;
}

void from_json(const json& j, GeneralConfig& config) {
  config.locale = j.value("locale", std::string("zh_CN"));
  config.thread_count = j.value("thread_count", 4);
}

}  // namespace parakeet_wx::config
