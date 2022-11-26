#include "KuwoConfigConverter.h"
#include "ByteContainerConverter.h"

NLOHMANN_JSON_NAMESPACE_BEGIN
using KuwoConfig = parakeet_crypto::decryption::config::KuwoConfig;
namespace utils = ::parakeet_crypto::utils;

void adl_serializer<KuwoConfig>::to_json(json& j, const KuwoConfig& data) {
  j["key"] = data.key;
}

void adl_serializer<KuwoConfig>::from_json(const json& j, KuwoConfig& data) {
  KuwoConfig def;
  data.key = j.value("key", def.key);
}
NLOHMANN_JSON_NAMESPACE_END
