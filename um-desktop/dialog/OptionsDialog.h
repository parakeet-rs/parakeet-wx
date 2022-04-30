#pragma once

#include "../ui/ui.h"

class OptionsDialog : public umd::ui_base::uiOptionsDialog {
 public:
  OptionsDialog(wxWindow* parent);

 protected:
  void OnPropertyGridChanged_KuGou(wxPropertyGridEvent& event) override;
};
