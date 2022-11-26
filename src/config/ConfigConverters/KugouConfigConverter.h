#pragma once

#include <parakeet-crypto/decryption/DecryptionManager.h>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN
template <>
struct adl_serializer<parakeet_crypto::decryption::config::KugouConfig> {
  static void to_json(json& j, const parakeet_crypto::decryption::config::KugouConfig& data);
  static void from_json(const json& j, parakeet_crypto::decryption::config::KugouConfig& data);
};
NLOHMANN_JSON_NAMESPACE_END
