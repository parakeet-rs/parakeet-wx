#pragma once

#include "utils/json/ByteContainerConverter.hpp"

#include <nlohmann/json.hpp>
#include <vector>

namespace parakeet_wx::config
{

struct XimalayaVariantConfig
{
    double init_value;
    double step_value;
    std::vector<uint8_t> content_key;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(XimalayaVariantConfig, init_value, step_value, content_key);

struct XimalayaConfig
{
    XimalayaVariantConfig x2m;
    XimalayaVariantConfig x3m;
};

using json = nlohmann::json;

inline void to_json(json &j, const XimalayaConfig &data)
{
    j = json{{"x2m", data.x2m}, {"x3m", data.x3m}};
}

inline void from_json(const json &j, XimalayaConfig &data)
{
    if (auto x2m = j.find("x2m"); x2m != j.end())
    {
        x2m.value().get_to(data.x2m);
    }
    else
    {
        data.x2m.content_key = j.value("x2m_content_key", data.x2m.content_key);
        if (auto x2m_table = j.find("x2m_scramble_table_parameters"); x2m_table != j.end())
        {
            auto &x2m_table_value = x2m_table.value();
            data.x2m.init_value = x2m_table_value.value("init_value", double{0.00});
            data.x2m.step_value = x2m_table_value.value("step_value", double{0.00});
        }
    }

    if (auto x3m = j.find("x3m"); x3m != j.end())
    {
        x3m.value().get_to(data.x3m);
    }
    else
    {
        data.x3m.content_key = j.value("x3m_content_key", data.x3m.content_key);
        if (auto x3m_table = j.find("x3m_scramble_table_parameters"); x3m_table != j.end())
        {
            auto &x3m_table_value = x3m_table.value();
            data.x3m.init_value = x3m_table_value.value("init_value", double{0.00});
            data.x3m.step_value = x3m_table_value.value("step_value", double{0.00});
        }
    }
}

} // namespace parakeet_wx::config
