#include "QMCConfigConverter.h"
#include "ByteContainerConverter.h"

NLOHMANN_JSON_NAMESPACE_BEGIN
using QMCConfig = parakeet_crypto::decryption::config::QMCConfig;
namespace utils = ::parakeet_crypto::utils;

void adl_serializer<QMCConfig>::to_json(json& j, const QMCConfig& data) {
  j["ekey_seed"] = data.ekey_seed;
  j["enc_v2_stage1_key"] = data.enc_v2_stage1_key;
  j["enc_v2_stage2_key"] = data.enc_v2_stage2_key;
  j["static_cipher_key"] = data.static_cipher_key;
}

void adl_serializer<QMCConfig>::from_json(const json& j, QMCConfig& data) {
  QMCConfig def;
  data.ekey_seed = j.value("ekey_seed", def.ekey_seed);
  data.enc_v2_stage1_key = j.value("enc_v2_stage1_key", def.enc_v2_stage1_key);
  data.enc_v2_stage2_key = j.value("enc_v2_stage2_key", def.enc_v2_stage2_key);
  data.static_cipher_key = j.value("static_cipher_key", def.static_cipher_key);
}
NLOHMANN_JSON_NAMESPACE_END
