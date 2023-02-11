#pragma once
#include "parakeet-crypto/qmc2/key_crypto.h"
#include "utils/json/ByteContainerConverter.hpp"

#include <nlohmann/json.hpp>

namespace parakeet_wx::config
{

struct QMCV1Config
{
    std::vector<uint8_t> key; // works with size 58/128/256
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(QMCV1Config, key);

struct QMCV2Config
{
    uint8_t seed;
    std::array<uint8_t, parakeet_crypto::qmc2::kEncV2KeyLen> enc_v2_key1;
    std::array<uint8_t, parakeet_crypto::qmc2::kEncV2KeyLen> enc_v2_key2;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(QMCV2Config, seed, enc_v2_key1, enc_v2_key2);

struct QMCConfig
{
    QMCV1Config qmc1;
    QMCV2Config qmc2;
};

using json = nlohmann::json;

inline void to_json(json &j, const QMCConfig &data)
{
    j = json{{"qmc1", data.qmc1}, {"qmc2", data.qmc2}};
}

inline void from_json(const json &j, QMCConfig &data)
{
    if (auto qmc1 = j.find("qmc1"); qmc1 != j.end())
    {
        qmc1.value().get_to(data.qmc1);
    }
    else
    {
        data.qmc1.key = j.value("static_cipher_key", data.qmc1.key);
    }

    if (auto qmc2 = j.find("qmc2"); qmc2 != j.end())
    {
        qmc2.value().get_to(data.qmc2);
    }
    else
    {
        data.qmc2.seed = j.value("ekey_seed", data.qmc2.seed);
        data.qmc2.enc_v2_key1 = j.value("enc_v2_stage1_key", data.qmc2.enc_v2_key1);
        data.qmc2.enc_v2_key2 = j.value("enc_v2_stage2_key", data.qmc2.enc_v2_key2);
    }
}

} // namespace parakeet_wx::config
