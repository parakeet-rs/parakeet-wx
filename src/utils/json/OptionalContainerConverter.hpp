#pragma once

#include <cstddef>
#include <cstdint>

#include <optional>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN

template <typename T> struct adl_serializer<std::optional<T>>
{
    inline static void to_json(json &j, const std::optional<T> &data)
    {
        if (data == std::nullopt)
        {
            j = nullptr;
        }
        else
        {
            j = json(*data);
        }
    }

    inline static void from_json(const json &j, std::optional<T> &data)
    {
        if (j.is_null())
        {
            data = std::nullopt;
        }
        else
        {
            data = j.get<T>();
        }
    }
};

NLOHMANN_JSON_NAMESPACE_END
