#pragma once

#include <parakeet-crypto/endian.h>

#include "../BytesSerialization.h"

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
      temp.resize(SIZE, 0);
      std::copy(temp.begin(), temp.end(), data.begin());
    } else {
      std::fill(data.begin(), data.end(), 0);
    }
  }
};

template <size_t SIZE>
struct adl_serializer<std::array<uint16_t, SIZE>> {
  inline static void to_json(json& j, const std::array<uint16_t, SIZE>& data) {
    std::array<uint16_t, SIZE> temp(data);

    std::transform(data.cbegin(), data.cend(), temp.begin(), ::parakeet_crypto::SwapHostToLittleEndian<uint16_t>);

    j = ::parakeet_wx::utils::SerializeBytes(
        std::span{reinterpret_cast<uint8_t*>(temp.data()), SIZE * sizeof(uint16_t)});
  }

  inline static void from_json(const json& j, std::array<uint16_t, SIZE>& data) {
    if (j.is_string()) {
      std::vector<uint8_t> temp = ::parakeet_wx::utils::DeserializeBytes(j.get<std::string>());
      temp.resize(SIZE * sizeof(uint16_t), 0);
      auto temp_ptr = std::span{reinterpret_cast<uint16_t*>(temp.data()), SIZE};
      std::transform(temp_ptr.begin(), temp_ptr.end(), data.begin(), ::parakeet_crypto::SwapLittleEndianToHost<uint16_t>);
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
