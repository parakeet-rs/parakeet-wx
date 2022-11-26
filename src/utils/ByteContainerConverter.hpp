#pragma once

#include "BytesSerialization.h"

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <array>
#include <vector>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN

template <size_t SIZE>
struct adl_serializer<std::array<uint8_t, SIZE>> {
  inline static void to_json(json& j, const std::array<uint8_t, SIZE>& data) {
    j = ::parakeet_wx::utils::SerializeBytes(data);
  }

  inline static void from_json(const json& j, std::array<uint8_t, SIZE>& data) {
    if (j.is_string()) {
      std::vector<uint8_t> temp = ::parakeet_wx::utils::DeserializeBytes(j.get<std::string>());
      temp.resize(data.size(), 0);
      std::copy(temp.begin(), temp.end(), data.begin());
    } else {
      std::fill(data.begin(), data.end(), 0);
    }
  }
};

template <>
struct adl_serializer<std::vector<uint8_t>> {
  inline static void to_json(json& j, const std::vector<uint8_t>& data) {
    j = ::parakeet_wx::utils::SerializeBytes(data);
  }
  inline static void from_json(const json& j, std::vector<uint8_t>& data) {
    if (j.is_string()) {
      data = ::parakeet_wx::utils::DeserializeBytes(j.get<std::string>());
    } else {
      data.resize(0);
    }
  }
};

NLOHMANN_JSON_NAMESPACE_END
