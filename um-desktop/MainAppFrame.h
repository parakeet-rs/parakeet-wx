#pragma once
#include "ui/ui.h"

namespace umd::ui {

class MainAppFrame : public umd::ui_base::uiMainAppFrame {
 public:
  MainAppFrame(wxWindow* parent, wxWindowID id = wxID_ANY);
};

}  // namespace umd::ui
