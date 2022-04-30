#include "OptionsDialog.h"

OptionsDialog::OptionsDialog(wxWindow* parent) : uiOptionsDialog(parent) {
  m_propGridKugou->Append(
      new wxLongStringProperty(_("Table 1 (hex)"), wxT("name1"), wxT("")));

  m_propGridKugou->Append(
      new wxLongStringProperty(_("Table 2 (hex)"), wxT("name2"), wxT("")));

  m_propGridKugou->Append(
      new wxLongStringProperty(_("Mask v2 (hex)"), wxT("name3"), wxT("")));

  m_propGridKugou->Append(
      new wxLongStringProperty(_("vprTable (hex)"), wxT("name4"), wxT("")));
}

void OptionsDialog::OnPropertyGridChanged_KuGou(wxPropertyGridEvent& event) {
  event.Skip();

  auto a = wxString("");
  a += event.GetPropertyName();
  a += "=";
  a += event.GetPropertyValue().GetString();

  wxMessageBox(a, wxT("hello?"));
}
