#include "AppConfigConverter.h"

using json = nlohmann::json;

namespace parakeet_wx::config {

void to_json(json& j, const AppConfig& config) {
  // TODO: Implement the conversion here.
  j = json{};
}

void from_json(const json& j, AppConfig& config) {
  // TODO: Implement the conversion here.
}

}  // namespace parakeet_wx::config
