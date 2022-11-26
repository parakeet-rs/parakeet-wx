#pragma once

#include <parakeet-crypto/decryption/DecryptionManager.h>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN
template <>
struct adl_serializer<parakeet_crypto::decryption::config::KuwoConfig> {
  static void to_json(json& j, const parakeet_crypto::decryption::config::KuwoConfig& data);
  static void from_json(const json& j, parakeet_crypto::decryption::config::KuwoConfig& data);
};
NLOHMANN_JSON_NAMESPACE_END
