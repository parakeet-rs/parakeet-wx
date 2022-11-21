#include "OptionsDialog.h"

#include "config/AppConfigStore.h"
#include "constants.h"
#include "utils/StringConvert.h"

#include <cstdlib>
#include <sstream>

constexpr int kModeUpdateControl = 1;
constexpr int kModeReadFromControl = 2;

template <int M>
void OptionsDialog::ConfigGlueCode() {
  namespace conv = parakeet_wx::utils::str_conv;
  auto& config = saved_config_;

#define DEFINE_CTRL_PROP(CONTROL, KEY)                                  \
  if (M == kModeUpdateControl) {                                        \
    std::string str_value;                                              \
    conv::ToString(str_value, KEY);                                     \
    CONTROL->SetValueFromString(wxString::FromUTF8(str_value.c_str())); \
  } else {                                                              \
    auto temp = CONTROL->GetValueAsString(wxPG_FULL_VALUE);             \
    auto u8_str = temp.utf8_str();                                      \
    conv::FromString(KEY, std::string(u8_str));                         \
  }

  {
    auto& desktop = config.desktop;
    DEFINE_CTRL_PROP(m_generalThreadPoolSize, desktop.general.thread_count);
    DEFINE_CTRL_PROP(m_generalLocale, desktop.general.locale);
  }

  {
    auto& d = config.decryption;
    DEFINE_CTRL_PROP(m_kugouSlotKey1, d.kugou.slot_key_1);
    DEFINE_CTRL_PROP(m_kugouV4SlotKeyExpansionTable, d.kugou.v4_slot_key_expansion_table);
    DEFINE_CTRL_PROP(m_kugouV4FileKeyExpansionTable, d.kugou.v4_file_key_expansion_table);

    DEFINE_CTRL_PROP(m_kuwoKey, d.kuwo.key);

    DEFINE_CTRL_PROP(m_neteaseNCMContentKeyAESKey, d.netease.key);

    DEFINE_CTRL_PROP(m_tencentEKeySeed, d.qmc.ekey_seed);
    DEFINE_CTRL_PROP(m_tencentStaticTable, d.qmc.static_cipher_key);
    DEFINE_CTRL_PROP(m_encV2Stage1Key, d.qmc.enc_v2_stage1_key);
    DEFINE_CTRL_PROP(m_encV2Stage2Key, d.qmc.enc_v2_stage2_key);

    DEFINE_CTRL_PROP(m_jooxUUID, d.joox.install_uuid);
    DEFINE_CTRL_PROP(m_jooxSalt, d.joox.salt);

    DEFINE_CTRL_PROP(m_xmlyX2MContentKey, d.ximalaya.x2m_content_key);
    DEFINE_CTRL_PROP(m_xmlyX2MScrambleTable, d.ximalaya.x2m_scramble_table);
    DEFINE_CTRL_PROP(m_xmlyX3MContentKey, d.ximalaya.x3m_content_key);
    DEFINE_CTRL_PROP(m_xmlyX3MScrambleTable, d.ximalaya.x3m_scramble_table);

    if (M == kModeUpdateControl) {
      if (d.ximalaya.x2m_scramble_table_parameters.has_value()) {
        m_xmlyX2MScrambleTableParamInit->SetValue(d.ximalaya.x2m_scramble_table_parameters->init_value);
        m_xmlyX2MScrambleTableParamStep->SetValue(d.ximalaya.x2m_scramble_table_parameters->step_value);
      }
      if (d.ximalaya.x3m_scramble_table_parameters.has_value()) {
        m_xmlyX3MScrambleTableParamInit->SetValue(d.ximalaya.x3m_scramble_table_parameters->init_value);
        m_xmlyX3MScrambleTableParamStep->SetValue(d.ximalaya.x3m_scramble_table_parameters->step_value);
      }
    } else {
      using namespace parakeet_crypto::decryption;

      auto x2m_scramble_table_init = m_xmlyX2MScrambleTableParamInit->GetValue().GetDouble();
      auto x2m_scramble_table_step = m_xmlyX2MScrambleTableParamStep->GetValue().GetDouble();
      if (x2m_scramble_table_init != 0 || x2m_scramble_table_step != 0) {
        d.ximalaya.x2m_scramble_table_parameters = ximalaya::XmlyScrambleTableParameter{
            .init_value = x2m_scramble_table_init,
            .step_value = x2m_scramble_table_step,
        };
      }

      auto x3m_scramble_table_init = m_xmlyX3MScrambleTableParamInit->GetValue().GetDouble();
      auto x3m_scramble_table_step = m_xmlyX3MScrambleTableParamStep->GetValue().GetDouble();
      if (x3m_scramble_table_init != 0 || x3m_scramble_table_step != 0) {
        d.ximalaya.x3m_scramble_table_parameters = ximalaya::XmlyScrambleTableParameter{
            .init_value = x3m_scramble_table_init,
            .step_value = x3m_scramble_table_step,
        };
      }
    }
  }
}

OptionsDialog::OptionsDialog(wxWindow* parent) : uiOptionsDialog(parent) {
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

void OptionsDialog::OnButtonClick_Cancel(wxCommandEvent& event) {
  Close();
}
void OptionsDialog::OnButtonClick_OK(wxCommandEvent& event) {
  auto config = parakeet_wx::config::AppConfigStore::GetInstance()->GetLoadedConfig();

  // TODO: Improve error handling.
  ConfigGlueCode<kModeReadFromControl>();
  saved_ = true;

  Close();
}
