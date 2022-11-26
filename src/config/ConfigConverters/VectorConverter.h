#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN

template <>
struct adl_serializer<std::vector<uint8_t>> {
  static void to_json(json& j, const std::vector<uint8_t>& data);
  static void from_json(const json& j, std::vector<uint8_t>& data);
};

NLOHMANN_JSON_NAMESPACE_END
