#include "MainAppFrame.h"

#include "../utils/AudioDecryptorManager.h"
#include "../utils/threading.h"
#include "OptionsDialog.h"

#include <wx/filedlg.h>
#include <wx/propgrid/propgrid.h>
#include <wx/wfstream.h>

#include <boost/chrono.hpp>
#include <boost/nowide/convert.hpp>
#include <boost/thread/thread.hpp>

#include <functional>

// TODO: remove this table
#include "__priv_table.h"
#include "um-crypto/kugou/KGMMaskGenerator.h"

const int kThreadCount = 4;

MainAppFrame::MainAppFrame(wxWindow* parent, wxWindowID id)
    : uiMainAppFrame(parent, id) {
  SetDropTarget(dynamic_cast<wxDropTarget*>(this));

  umc::kugou::KGMMaskGenerator::GetInstance()->SetTable(t1, t2, v2);

  m_decryptLogs->InsertColumn(0, wxT(""), wxLIST_FORMAT_LEFT, 100);

  m_btnAddDir->Hide();
#if NDEBUG
  m_btnOptions->Hide();
#else
  SetTitle(GetTitle() + "  [" + _("DEBUG Build") + "]");
#endif
}

void MainAppFrame::uiMainAppFrameOnSize(wxSizeEvent& event) {
  event.Skip();

  int w, h;
  this->m_decryptLogs->GetSize(&w, &h);
  this->m_decryptLogs->SetColumnWidth(0, w);
};

void MainAppFrame::OnBtnClickOptions(wxCommandEvent& event) {
  auto optionsDialog = new OptionsDialog(this);
  optionsDialog->ShowModal();
  optionsDialog->Destroy();
}

void MainAppFrame::SetDecryptionInProgress(bool in_progress) {
  m_btnClearLogs->Enable(!in_progress);
  m_btnProcess->Enable(!in_progress);
  m_btnOptions->Enable(!in_progress);
}

#define QMCv2_FILTER "*.mgg;*.mgg0;*.mgg1;*.mflac;*.mflac0;*.mflac1"
#define Kugou_FILTER "*.kgm;*.vpr"
#define ALL_SUPPORTED_FILTER QMCv2_FILTER ";" Kugou_FILTER

void MainAppFrame::OnButtonClick_AddFile(wxCommandEvent& event) {
  event.Skip();

  wxString filter;
  filter += _("All supported files");
  filter += wxT("|" ALL_SUPPORTED_FILTER "|");
  filter += _("QMCv2 files (*.mgg;*.mflac)");
  filter += wxT("|" QMCv2_FILTER "|");
  filter += _("Kugou music (*.kgm;*.vpr)");
  filter += wxT("|" Kugou_FILTER "|");
  filter += _("All files (*.*)|*");

  wxFileDialog openFileDialog(this, _("Open encrypted music files"), "", "",
                              filter,
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

  if (openFileDialog.ShowModal() == wxID_CANCEL)
    return;  // the user changed idea...

  wxArrayString paths;
  openFileDialog.GetPaths(paths);
  HandleAddFilesToQueue(paths);
}

bool MainAppFrame::OnDropFiles(wxCoord x,
                               wxCoord y,
                               const wxArrayString& filenames) {
  HandleAddFilesToQueue(filenames);
  return true;
}

void MainAppFrame::HandleAddFilesToQueue(const wxArrayString& filenames) {
  auto len = filenames.GetCount();
  for (int i = 0; i < len; i++) {
    wxListItem new_item;
    new_item.SetText(wxT("  -  ") + filenames.Item(i));
    new_item.SetId(m_decryptLogs->GetItemCount());

    auto rowIndex = m_decryptLogs->InsertItem(new_item);
    file_entries_.push_back(std::make_shared<FileEntry>(FileEntry{
        FileProcessStatus::kNotProcessed,
        filenames.Item(i),
        rowIndex,
    }));
  }
}

void MainAppFrame::OnButtonClick_AddDirectory(wxCommandEvent& event) {
  event.Skip();
}

void MainAppFrame::OnButtonClick_ClearLogs(wxCommandEvent& event) {
  for (int i = file_entries_.size() - 1; i >= 0; i--) {
    m_decryptLogs->DeleteItem(0);
  }

  file_entries_.clear();
  file_entry_process_idx_.store(0);
  file_entry_complete_count_.store(0);
}

void MainAppFrame::OnButtonClick_ProcessFiles(wxCommandEvent& event) {
  SetDecryptionInProgress(true);

  const int len = file_entries_.size() - file_entry_complete_count_;
  if (len == 0) {
    SetDecryptionInProgress(false);
    return;
  }

  for (int i = len - 1; i >= 0; i--) {
    umd::io_service.post([this]() { this->ProcessNextFile(); });
  }
}

void MainAppFrame::UpdateFileStatus(int idx, FileProcessStatus status) {
  std::lock_guard<std::mutex> guard(update_status_mutex_);

  auto entry = file_entries_.at(idx);
  entry->status = status;

  wxString status_text;

  switch (status) {
    case FileProcessStatus::kNotProcessed:
      status_text = _("Ready: ");
      break;
    case FileProcessStatus::kProcessedOk:
      status_text = _("OK: ");
      break;
    case FileProcessStatus::kProcessFailed:
      status_text = _("FAIL: ");
      break;
    case FileProcessStatus::kProcessing:
      status_text = _("  ...  ");
      break;
    case FileProcessStatus::kProcessNotSupported:
      status_text = _("Unsupported: ");
      break;

    default:
      status_text.Printf(_("Unknown state (code %d)"), (int)status);
      break;
  }

  status_text += "    " + entry->file_path;

  m_decryptLogs->SetItem(entry->index, 0, status_text);
}

using umd::utils::EncryptionType;

void MainAppFrame::ProcessNextFile() {
  auto current_index = file_entry_process_idx_.fetch_add(1);
  if (current_index >= file_entries_.size()) {
    file_entry_process_idx_.fetch_sub(1);
    return;
  }

  auto entry = file_entries_.at(current_index);

  UpdateFileStatus(current_index, FileProcessStatus::kProcessing);

  umd::utils::AudioDecryptorManager decryptor;
  decryptor.Open(boost::nowide::narrow(entry->file_path.t_str()));

  auto encryption = decryptor.SniffEncryption();
  if (encryption == EncryptionType::kNotEncrypted) {
    UpdateFileStatus(current_index, FileProcessStatus::kProcessNotSupported);
  } else if (encryption == EncryptionType::kUnsupported) {
    UpdateFileStatus(current_index, FileProcessStatus::kProcessNotSupported);
  } else {
    // TODO: show encryption type
    if (decryptor.DecryptAudioFile()) {
      UpdateFileStatus(current_index, FileProcessStatus::kProcessedOk);
    } else {
      UpdateFileStatus(current_index, FileProcessStatus::kProcessFailed);
    }
  }

  OnProcessSingleFileComplete();
}

void MainAppFrame::OnProcessSingleFileComplete() {
  int completion_count = file_entry_complete_count_.fetch_add(1) + 1;
  if (completion_count == file_entries_.size()) {
    SetDecryptionInProgress(false);
  }
}
