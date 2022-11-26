#pragma once

#include <parakeet-crypto/decryption/DecryptionManager.h>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN
template <>
struct adl_serializer<parakeet_crypto::decryption::config::QMCConfig> {
  static void to_json(json& j, const parakeet_crypto::decryption::config::QMCConfig& data);
  static void from_json(const json& j, parakeet_crypto::decryption::config::QMCConfig& data);
};
NLOHMANN_JSON_NAMESPACE_END
