#include "app.h"

#include "ui/ui.h"

IMPLEMENT_APP(umDesktopApp)

// This is executed upon startup, like 'main()' in non-wxWidgets programs.
bool umDesktopApp::OnInit() {
  wxFrame *frame = new ui::uiMainAppFrame(nullptr);
  frame->Show(true);
  SetTopWindow(frame);
  return true;
}
