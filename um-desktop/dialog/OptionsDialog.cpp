#include "OptionsDialog.h"

#include "../config/AppConfigStore.h"
#include "../constants.h"
#include "../utils/StringConvert.h"

#include <cstdlib>
#include <sstream>

constexpr int kModeUpdateControl = 1;
constexpr int kModeReadFromControl = 2;

template <int M>
void OptionsDialog::ConfigGlueCode() {
  namespace conv = umd::utils::str_conv;
  auto& config = saved_config_;

#define DEFINE_CTRL_PROP(CONTROL, KEY)                                     \
  if (M == kModeUpdateControl) {                                           \
    umc::Str str_value;                                                    \
    conv::ToString(str_value, KEY);                                        \
    CONTROL->SetValueFromString(wxString::FromUTF8(str_value));            \
  } else {                                                                 \
    auto value = CONTROL->GetValueAsString(wxPG_FULL_VALUE).utf8_string(); \
    conv::FromString(KEY, value);                                          \
  }

  {
    auto& desktop = config.desktop;
    DEFINE_CTRL_PROP(m_generalThreadPoolSize, desktop.general.thread_count);
    DEFINE_CTRL_PROP(m_generalLocale, desktop.general.locale);
  }

  {
    auto& d = config.decryption;
    DEFINE_CTRL_PROP(m_kugouT1, d.kugou.t1);
    DEFINE_CTRL_PROP(m_kugouT2, d.kugou.t2);
    DEFINE_CTRL_PROP(m_kugouV2, d.kugou.v2);
    DEFINE_CTRL_PROP(m_kugouVPR, d.kugou.vpr_key);

    DEFINE_CTRL_PROP(m_kuwoKey, d.kuwo.key);

    DEFINE_CTRL_PROP(m_tencentEKeySeed, d.qmc.ekey_seed);
    DEFINE_CTRL_PROP(m_tencentStaticTable, d.qmc.static_cipher_key);

    DEFINE_CTRL_PROP(m_jooxUUID, d.joox.install_uuid);
    DEFINE_CTRL_PROP(m_jooxSalt, d.joox.salt);

    DEFINE_CTRL_PROP(m_xmlyX2MContentKey, d.ximalaya.x2m_content_key);
    DEFINE_CTRL_PROP(m_xmlyX2MScrambleTable, d.ximalaya.x2m_scramble_table);
    DEFINE_CTRL_PROP(m_xmlyX3MContentKey, d.ximalaya.x3m_content_key);
    DEFINE_CTRL_PROP(m_xmlyX3MScrambleTable, d.ximalaya.x3m_scramble_table);
  }
}

OptionsDialog::OptionsDialog(wxWindow* parent) : uiOptionsDialog(parent) {
  wxString title;
  title.Printf(_("%s - Options"), LOCALISED_APP_NAME);
  SetTitle(title);

  auto inst = umd::config::AppConfigStore::GetInstance();
  saved_config_ = inst->GetLoadedConfig();
  ConfigGlueCode<kModeUpdateControl>();
}

void OptionsDialog::OnButtonClick_Cancel(wxCommandEvent& event) {
  Close();
}
void OptionsDialog::OnButtonClick_OK(wxCommandEvent& event) {
  auto config = umd::config::AppConfigStore::GetInstance()->GetLoadedConfig();

  // TODO: Improve error handling.
  ConfigGlueCode<kModeReadFromControl>();
  saved_ = true;

  Close();
}
