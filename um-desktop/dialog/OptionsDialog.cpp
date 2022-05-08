#include "OptionsDialog.h"

#include "../config/AppConfigStore.h"
#include "../constants.h"

#include <um-crypto/utils/StringHelper.h>
#include <um-crypto/utils/hex.h>

#include <cstdlib>
#include <sstream>

using umc::utils::Hex;
using umc::utils::UnescapeCharSequence;
using umc::utils::Unhex;

template <class ConfigType>
struct ConfigConvert {
  static std::string ConfigToText(const ConfigType& input);
  static ConfigType TextToConfig(const std::string& input);
};

template <>
struct ConfigConvert<umc::Vec<umc::u8>> {
  static std::string ConfigToText(const umc::Vec<umc::u8>& input) {
    return Hex(input);
  }
  static umc::Vec<umc::u8> TextToConfig(const std::string& input) {
    return Unhex(UnescapeCharSequence(input));
  };
};

template <>
struct ConfigConvert<int> {
  static std::string ConfigToText(const int& input) {
    std::stringstream ss;
    ss << input;
    return ss.str();
  }
  static int TextToConfig(const std::string& input) {
    return atoi(input.c_str());
  };
};

constexpr int kModeUpdateControl = 1;
constexpr int kModeReadFromControl = 2;

template <int M>
void OptionsDialog::ConfigGlueCode() {
  auto& config = saved_config_;

#define DEFINE_CTRL_PROP(CONTROL, KEY)                                    \
  if (M == kModeUpdateControl) {                                          \
    CONTROL->SetValueFromString(                                          \
        ConfigConvert<decltype(KEY)>::ConfigToText(KEY));                 \
  } else {                                                                \
    KEY = ConfigConvert<decltype(KEY)>::TextToConfig(                     \
        std::string(CONTROL->GetValueAsString(wxPG_FULL_VALUE).c_str())); \
  }

  DEFINE_CTRL_PROP(m_generalThreadPoolSize, config.general.thread_count);

  DEFINE_CTRL_PROP(m_kugouT1, config.kugou.t1);
  DEFINE_CTRL_PROP(m_kugouT2, config.kugou.t2);
  DEFINE_CTRL_PROP(m_kugouV2, config.kugou.v2);
  DEFINE_CTRL_PROP(m_kugouVPR, config.kugou.vpr_key);

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
  m_notebookOptions->DeletePage(2);  // unused: ximalaya
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
