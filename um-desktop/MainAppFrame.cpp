#include "MainAppFrame.h"

namespace umd::ui {

MainAppFrame::MainAppFrame(wxWindow* parent, wxWindowID id)
    : uiMainAppFrame(parent, id) {
  this->m_fileListTable->InsertColumn(0, _("Col1"), wxLIST_FORMAT_LEFT, -1);
  this->m_fileListTable->InsertColumn(1, _("Col2"), wxLIST_FORMAT_LEFT, 80);

  auto row1 = this->m_fileListTable->InsertItem(0, _("cell 0,0"));
  this->m_fileListTable->SetItem(row1, 1, _("cell 0,1"));

  auto row2 = this->m_fileListTable->InsertItem(0, _("cell 1,0"));
  this->m_fileListTable->SetItem(row2, 1, _("cell 1,1"));
}

}  // namespace umd::ui
