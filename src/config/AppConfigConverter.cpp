#include "AppConfigConverter.h"

using json = nlohmann::json;

namespace parakeet_wx::config {

#pragma region "JSON GlueCode - Guess I have to have some :/"
template <typename T>
inline void get_optional_json_field(const json& j, const char* field, T& value, T defaultValue) {
  if (j.contains(field)) {
    j.at(field).get_to(value);
  } else {
    value = defaultValue;
  }
}

template <typename T>
inline void get_optional_json_field(const json& j, const char* field, T& value) {
  if (j.contains(field)) {
    j.at(field).get_to(value);
  }
}
#pragma endregion

void to_json(json& j, const AppConfig& config) {
  j = json{
      {"general", config.desktop.general},

      // TODO: Serialise decryption configuration
  };
}

void from_json(const json& j, AppConfig& config) {
  config = AppConfig{};
  get_optional_json_field(j, "general", config.desktop.general);

  // TODO: Parse decryption configuration
}

void to_json(json& j, const GeneralConfig& config) {
  j = json{
      {"locale", config.locale},
      {"thread_count", config.thread_count},
  };
}

void from_json(const json& j, GeneralConfig& config) {
  config = GeneralConfig{};
  get_optional_json_field(j, "locale", config.locale, std::string("zh_CN"));
  get_optional_json_field(j, "thread_count", config.thread_count, 4);
}

}  // namespace parakeet_wx::config
