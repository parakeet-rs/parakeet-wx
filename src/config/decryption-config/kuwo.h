#pragma once
#include "parakeet-crypto/transformer/kuwo.h"
#include "utils/json/ByteContainerConverter.hpp"

#include <nlohmann/json.hpp>

namespace parakeet_wx::config
{

struct KuwoConfig
{
    std::array<uint8_t, parakeet_crypto::transformer::kKuwoDecryptionKeySize> key;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(KuwoConfig, key);

} // namespace parakeet_wx::config
