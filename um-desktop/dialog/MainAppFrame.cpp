#include "MainAppFrame.h"
#include "OptionsDialog.h"

#include <wx/propgrid/propgrid.h>

const int state_width = 80;

MainAppFrame::MainAppFrame(wxWindow* parent, wxWindowID id)
    : uiMainAppFrame(parent, id) {
  m_decryptLogs->InsertColumn(0, _("State"), wxLIST_FORMAT_LEFT, state_width);
  m_decryptLogs->InsertColumn(1, _("Path"), wxLIST_FORMAT_LEFT, 200);

  auto row1 = m_decryptLogs->InsertItem(0, _("-"));
  m_decryptLogs->SetItem(row1, 1, wxT("C:/a.qmc"));

  for (int i = 0; i < 100; i++) {
    auto row2 = m_decryptLogs->InsertItem(0, _("Waiting..."));
    m_decryptLogs->SetItem(row2, 1, _("C:/b.qmc"));
  }
}

void MainAppFrame::uiMainAppFrameOnSize(wxSizeEvent& event) {
  event.Skip();

  int w, h;
  this->m_decryptLogs->GetSize(&w, &h);
  this->m_decryptLogs->SetColumnWidth(1, w - state_width);
};

void MainAppFrame::OnBtnClickOptions(wxCommandEvent& event) {
  auto optionsDialog = new OptionsDialog(this);
  optionsDialog->ShowModal();
  optionsDialog->Destroy();
}

void MainAppFrame::enableLogClearButton(bool enabled) {
  m_btnClearLogs->Enable(enabled);
}
