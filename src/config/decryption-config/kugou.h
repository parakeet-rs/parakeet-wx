#pragma once
#include "parakeet-crypto/transformer/kgm.h"

#include "utils/json/ByteContainerConverter.hpp"
#include "utils/json/MapContainerConverter.hpp"

#include <nlohmann/json.hpp>

namespace parakeet_wx::config
{

using KugouConfig = parakeet_crypto::transformer::KGMConfig;

} // namespace parakeet_wx::config

NLOHMANN_JSON_NAMESPACE_BEGIN
template <> struct adl_serializer<::parakeet_crypto::transformer::KGMConfigV4>
{
    static void to_json(json &j, const ::parakeet_crypto::transformer::KGMConfigV4 &data)
    {
        j["file_key_table"] = data.file_key_table;
        j["slot_key_table"] = data.slot_key_table;
    }
    static void from_json(const json &j, ::parakeet_crypto::transformer::KGMConfigV4 &data)
    {
        data.file_key_table = j.value("file_key_table", data.file_key_table);
        data.slot_key_table = j.value("slot_key_table", data.slot_key_table);
    }
};

template <> struct adl_serializer<::parakeet_wx::config::KugouConfig>
{
    static void to_json(json &j, const ::parakeet_wx::config::KugouConfig &data)
    {
        j = json::object({});
        j["slot_keys"] = data.slot_keys;
        j["v4"] = data.v4;
    }

    static void from_json(const json &j, ::parakeet_wx::config::KugouConfig &data)
    {
        if (auto slot_keys = j.find("slot_keys"); slot_keys != j.end())
        {
            data.slot_keys = slot_keys->get<std::map<uint32_t, std::vector<uint8_t>>>();
        }
        else if (auto slot_key_1 = j.find("slot_key_1"); slot_key_1 != j.end())
        {
            data.slot_keys[1] = slot_key_1->get<std::vector<uint8_t>>();
        }

        if (auto v4 = j.find("v4"); v4 != j.end())
        {
            data.v4 = v4->get<::parakeet_crypto::transformer::KGMConfigV4>();
        }
        else
        {
            ::parakeet_wx::config::KugouConfig def{};
            data.v4.file_key_table = j.value("v4_file_key_expansion_table", def.v4.file_key_table);
            data.v4.slot_key_table = j.value("v4_slot_key_expansion_table", def.v4.file_key_table);
        }
    }
};
NLOHMANN_JSON_NAMESPACE_END
