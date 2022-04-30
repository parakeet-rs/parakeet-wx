#pragma once
#include "../ui/ui.h"

class MainAppFrame : public umd::ui_base::uiMainAppFrame {
 public:
  MainAppFrame(wxWindow* parent, wxWindowID id = wxID_ANY);
  void enableLogClearButton(bool enable);

 protected:
  void uiMainAppFrameOnSize(wxSizeEvent& event) override;
  void OnBtnClickOptions(wxCommandEvent& event) override;
};
