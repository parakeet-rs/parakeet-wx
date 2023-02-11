#pragma once

#include "utils/BytesSerialization.h"

#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <array>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN

template <> struct adl_serializer<std::map<uint32_t, std::vector<uint8_t>>>
{
    inline static void to_json(json &j, const std::map<uint32_t, std::vector<uint8_t>> &data)
    {
        j = json::object({});
        for (auto &it : data)
        {
            j[std::to_string(it.first)] = it.second;
        }
    }

    inline static void from_json(const json &j, std::map<uint32_t, std::vector<uint8_t>> &data)
    {
        data = {};
        if (j.is_object())
        {
            for (auto it = j.begin(); it != j.end(); ++it)
            {
                data[static_cast<uint32_t>(std::stol(it.key().c_str()))] = it.value().get<std::vector<uint8_t>>();
            }
        }
    }
};

NLOHMANN_JSON_NAMESPACE_END
