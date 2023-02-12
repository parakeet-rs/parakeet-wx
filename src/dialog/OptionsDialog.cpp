#include "OptionsDialog.h"

#include "config/AppConfigStore.h"
#include "constants.h"
#include "utils/StringConvert.h"

#include <cstdlib>
#include <sstream>

constexpr int kModeUpdateControl = 1;
constexpr int kModeReadFromControl = 2;

template <int M> void OptionsDialog::ConfigGlueCode()
{
    namespace conv = parakeet_wx::utils::str_conv;
    auto &config = saved_config_;

#define DEFINE_CTRL_PROP(CONTROL, KEY)                                                                                 \
    if (M == kModeUpdateControl)                                                                                       \
    {                                                                                                                  \
        std::string str_value;                                                                                         \
        conv::ToString(str_value, KEY);                                                                                \
        CONTROL->SetValueFromString(wxString::FromUTF8(str_value.c_str()));                                            \
    }                                                                                                                  \
    else                                                                                                               \
    {                                                                                                                  \
        auto temp = CONTROL->GetValueAsString(wxPG_FULL_VALUE);                                                        \
        auto u8_str = temp.utf8_str();                                                                                 \
        conv::FromString(KEY, std::string(u8_str));                                                                    \
    }

    {
        auto &desktop = config.desktop;
        DEFINE_CTRL_PROP(m_generalThreadPoolSize, desktop.general.thread_count);
        DEFINE_CTRL_PROP(m_generalLocale, desktop.general.locale);
    }

    {
        auto &d = config;
        if (d.kugou.slot_keys.find(1) == d.kugou.slot_keys.end())
        {
            d.kugou.slot_keys[1] = {};
        }
        DEFINE_CTRL_PROP(m_kugouSlotKey1, d.kugou.slot_keys[1]);
        DEFINE_CTRL_PROP(m_kugouV4SlotKeyExpansionTable, d.kugou.v4.slot_key_table);
        DEFINE_CTRL_PROP(m_kugouV4FileKeyExpansionTable, d.kugou.v4.file_key_table);

        DEFINE_CTRL_PROP(m_kuwoKey, d.kuwo.key);

        DEFINE_CTRL_PROP(m_neteaseNCMContentKeyAESKey, d.netease.key);

        DEFINE_CTRL_PROP(m_tencentStaticTable, d.qmc.qmc1.key);

        DEFINE_CTRL_PROP(m_tencentEKeySeed, d.qmc.qmc2.seed);
        DEFINE_CTRL_PROP(m_encV2Stage1Key, d.qmc.qmc2.enc_v2_key1);
        DEFINE_CTRL_PROP(m_encV2Stage2Key, d.qmc.qmc2.enc_v2_key2);

        DEFINE_CTRL_PROP(m_jooxUUID, d.joox.install_uuid);
        DEFINE_CTRL_PROP(m_jooxSalt, d.joox.salt);

        DEFINE_CTRL_PROP(m_xmlyX2MContentKey, d.ximalaya.x2m.content_key);
        DEFINE_CTRL_PROP(m_xmlyX3MContentKey, d.ximalaya.x3m.content_key);

        if constexpr (M == kModeUpdateControl)
        {
            m_xmlyX2MScrambleTableParamInit->SetValue(d.ximalaya.x2m.init_value);
            m_xmlyX2MScrambleTableParamStep->SetValue(d.ximalaya.x2m.step_value);

            m_xmlyX3MScrambleTableParamInit->SetValue(d.ximalaya.x3m.init_value);
            m_xmlyX3MScrambleTableParamStep->SetValue(d.ximalaya.x3m.step_value);
        }
        else
        {
            d.ximalaya.x2m.init_value = m_xmlyX2MScrambleTableParamInit->GetValue().GetDouble();
            d.ximalaya.x2m.step_value = m_xmlyX2MScrambleTableParamStep->GetValue().GetDouble();

            d.ximalaya.x3m.init_value = m_xmlyX3MScrambleTableParamInit->GetValue().GetDouble();
            d.ximalaya.x3m.step_value = m_xmlyX3MScrambleTableParamStep->GetValue().GetDouble();
        }
    }
}

OptionsDialog::OptionsDialog(wxWindow *parent) : uiOptionsDialog(parent)
{
    // Rescale for HiDPI support
    SetClientSize(FromDIP(GetClientSize()));
    CenterOnParent();

    // Set 6-digit precisions
    m_xmlyX2MScrambleTableParamInit->SetAttribute(wxPG_FLOAT_PRECISION, 6);
    m_xmlyX2MScrambleTableParamStep->SetAttribute(wxPG_FLOAT_PRECISION, 6);
    m_xmlyX3MScrambleTableParamInit->SetAttribute(wxPG_FLOAT_PRECISION, 6);
    m_xmlyX3MScrambleTableParamStep->SetAttribute(wxPG_FLOAT_PRECISION, 6);

    wxString title;
    title.Printf(_("%s - Options"), LOCALISED_APP_NAME);
    SetTitle(title);

    auto inst = parakeet_wx::config::AppConfigStore::GetInstance();
    saved_config_ = inst->GetLoadedConfig();
    ConfigGlueCode<kModeUpdateControl>();
}

void OptionsDialog::OnButtonClick_Cancel(wxCommandEvent &event)
{
    Close();
}
void OptionsDialog::OnButtonClick_OK(wxCommandEvent &event)
{
    auto config = parakeet_wx::config::AppConfigStore::GetInstance()->GetLoadedConfig();

    // TODO: Improve error handling.
    ConfigGlueCode<kModeReadFromControl>();
    saved_ = true;

    Close();
}
