#include "AppConfigConverter.h"

using json = nlohmann::json;

namespace parakeet_wx::config
{

void to_json(json &j, const AppConfig &config)
{
    j["general"] = config.desktop.general;

    j["kugou"] = config.kugou;
    j["kuwo"] = config.kuwo;
    j["netease"] = config.netease;
    j["qmc"] = config.qmc;
    j["joox"] = config.joox;
    j["ximalaya"] = config.ximalaya;
}

void from_json(const json &j, AppConfig &config)
{
    config.desktop.general = j.value("general", config.desktop.general);

    config.kugou = j.value("kugou", config.kugou);
    config.kuwo = j.value("kuwo", config.kuwo);
    config.netease = j.value("netease", config.netease);
    config.qmc = j.value("qmc", config.qmc);
    config.joox = j.value("joox", config.joox);
    config.ximalaya = j.value("ximalaya", config.ximalaya);
}

void to_json(json &j, const GeneralConfig &config)
{
    j["locale"] = config.locale;
    j["thread_count"] = config.thread_count;
}

void from_json(const json &j, GeneralConfig &config)
{
    GeneralConfig def;
    config.locale = j.value("locale", def.locale);
    config.thread_count = j.value("thread_count", def.thread_count);
}

} // namespace parakeet_wx::config
