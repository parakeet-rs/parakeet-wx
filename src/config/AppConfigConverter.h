#pragma once

#include "AppConfigTypes.h"

#include <nlohmann/json.hpp>

namespace parakeet_wx::config
{
using json = ::nlohmann::json;

void to_json(json &j, const AppConfig &config);
void from_json(const json &j, AppConfig &config);

void to_json(json &j, const GeneralConfig &config);
void from_json(const json &j, GeneralConfig &config);

} // namespace parakeet_wx::config
