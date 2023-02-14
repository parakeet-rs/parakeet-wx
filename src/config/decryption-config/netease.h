#pragma once
#include "parakeet-crypto/transformer/ncm.h"
#include "utils/json/ByteContainerConverter.hpp"

#include <nlohmann/json.hpp>

namespace parakeet_wx::config
{

struct NeteaseConfig
{
    std::array<uint8_t, parakeet_crypto::transformer::kNCMContentKeySize> key;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(NeteaseConfig, key);

} // namespace parakeet_wx::config
