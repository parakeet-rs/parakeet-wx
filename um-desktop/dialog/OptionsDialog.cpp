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
    CONTROL->SetValueFromString(wxString::FromUTF8(conv::ToString(KEY)));  \
  } else {                                                                 \
    auto value = CONTROL->GetValueAsString(wxPG_FULL_VALUE).utf8_string(); \
    conv::FromString(KEY, value);                                          \
  }

  DEFINE_CTRL_PROP(m_generalThreadPoolSize, config.general.thread_count);

  DEFINE_CTRL_PROP(m_kugouT1, config.kugou.t1);
  DEFINE_CTRL_PROP(m_kugouT2, config.kugou.t2);
  DEFINE_CTRL_PROP(m_kugouV2, config.kugou.v2);
  DEFINE_CTRL_PROP(m_kugouVPR, config.kugou.vpr_key);

  DEFINE_CTRL_PROP(m_tencentEKeySeed, config.tencent.ekey_seed);
  DEFINE_CTRL_PROP(m_tencentStaticTable, config.tencent.static_key);

  DEFINE_CTRL_PROP(m_xmlyX2MContentKey, config.xmly.x2m_content_key);
  DEFINE_CTRL_PROP(m_xmlyX3MContentKey, config.xmly.x3m_content_key);
  DEFINE_CTRL_PROP(m_xmlyX3MScrambleKey, config.xmly.x3m_scramble_indexes);
}

OptionsDialog::OptionsDialog(wxWindow* parent) : uiOptionsDialog(parent) {
  wxString title;
  title.Printf(_("%s - Options"), LOCALISED_APP_NAME);
  SetTitle(title);

  auto inst = umd::config::AppConfigStore::GetInstance();
  saved_config_ = inst->GetLoadedConfig();
  ConfigGlueCode<kModeUpdateControl>();

#if NDEBUG
  m_notebookOptions->DeletePage(0);  // unused: general
#endif
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
