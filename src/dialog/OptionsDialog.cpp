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

#define DEFINE_CTRL_PROP(CONTROL, KEY)                                         \
  if (M == kModeUpdateControl) {                                               \
    std::string str_value;                                                     \
    conv::ToString(str_value, KEY);                                            \
    CONTROL->SetValueFromString(wxString::FromUTF8(str_value.c_str()));        \
  } else {                                                                     \
    const char* value = CONTROL->GetValueAsString(wxPG_FULL_VALUE).utf8_str(); \
    conv::FromString(KEY, std::string(value));                                 \
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
  }
}

OptionsDialog::OptionsDialog(wxWindow* parent) : uiOptionsDialog(parent) {
  // Rescale for HiDPI support
  SetClientSize(FromDIP(GetClientSize()));
  CenterOnParent();

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
