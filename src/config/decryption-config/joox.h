#pragma once
#include "parakeet-crypto/transformer/joox.h"

#include "utils/json/ByteContainerConverter.hpp"

#include <nlohmann/json.hpp>

namespace parakeet_wx::config
{

using JooxConfig = ::parakeet_crypto::transformer::JooxConfig;

} // namespace parakeet_wx::config

NLOHMANN_JSON_NAMESPACE_BEGIN
template <> struct adl_serializer<::parakeet_crypto::transformer::JooxConfig>
{
    static void to_json(json &j, const ::parakeet_crypto::transformer::JooxConfig &data)
    {
        j = json{
            {"install_uuid", data.install_uuid},
            {"salt", data.salt},
        };
    }
    static void from_json(const json &j, ::parakeet_crypto::transformer::JooxConfig &data)
    {
        data.install_uuid = j.value("install_uuid", data.install_uuid);
        data.salt = j.value("salt", data.salt);
    }
};

NLOHMANN_JSON_NAMESPACE_END
