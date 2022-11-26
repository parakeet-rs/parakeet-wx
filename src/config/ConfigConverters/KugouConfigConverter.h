#pragma once

#include <parakeet-crypto/decryption/DecryptionManager.h>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN
using KugouConfig = parakeet_crypto::decryption::config::KugouConfig;

template <>
struct adl_serializer<KugouConfig> {
  static void to_json(json& j, const KugouConfig& data);
  static void from_json(const json& j, KugouConfig& data);
};
NLOHMANN_JSON_NAMESPACE_END
