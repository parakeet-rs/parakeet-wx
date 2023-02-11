#pragma once

// #include <parakeet-crypto/decryption/DecryptionManager.h>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "decryption-config/joox.h"
#include "decryption-config/kugou.h"
#include "decryption-config/kuwo.h"
#include "decryption-config/netease.h"
#include "decryption-config/qmc.h"
#include "decryption-config/ximalaya.h"

namespace parakeet_wx::config
{

struct GeneralConfig
{
    int thread_count;
    std::string locale;

    GeneralConfig() : thread_count(4), locale(std::string("zh_CN"))
    {
    }
};

struct DesktopConfig
{
    GeneralConfig general;
};

struct AppConfig
{
    DesktopConfig desktop;
    JooxConfig joox;
    KugouConfig kugou;
    KuwoConfig kuwo;
    NeteaseConfig netease;
    QMCConfig qmc;
    XimalayaConfig ximalaya;
};

} // namespace parakeet_wx::config
