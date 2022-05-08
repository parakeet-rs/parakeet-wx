#include "MainAppFrame.h"
#include "OptionsDialog.h"

#include "../constants.h"
#include "../utils/AudioDecryptorManager.h"
#include "../utils/MakeArray.h"
#include "../utils/threading.h"

#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/propgrid/propgrid.h>
#include <wx/wfstream.h>

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

#include <functional>
#include <thread>

#include <um-crypto/kugou/KGMMaskGenerator.h>
#include <um-crypto/ximalaya.h>

using boost::chrono::system_clock;

bool MainAppDropTarget::OnDropFiles(wxCoord x,
                                    wxCoord y,
                                    const wxArrayString& file_paths) {
  app_frame_->HandleAddFilesToQueue(file_paths);
  return true;
}

void MainAppFrame::OnThreadEvent(wxThreadEvent& event) {
  if (!main_thread_runner_.HandleMainThreadEvent(event)) {
    event.Skip();
  }
}

MainAppFrame::~MainAppFrame() {
  Unbind(wxEVT_THREAD, &MainAppFrame::OnThreadEvent, this);
}

MainAppFrame::MainAppFrame(wxWindow* parent, wxWindowID id)
    : uiMainAppFrame(parent, id) {
  // Bootstrap Multi-thread handling
  main_thread_runner_.SetMainThreadRunnerEventHandler(GetEventHandler());
  Bind(wxEVT_THREAD, &MainAppFrame::OnThreadEvent, this);

  // Bootstrap drag & drop
  SetDropTarget(new MainAppDropTarget(this));

  // Apply config
  umd::config::AppConfigStore::GetInstance()->LoadConfigFromDisk();
  ApplyConfigFromStore();

  // Setup decryptions logs
  m_decryptLogs->InsertColumn(0, wxT(""), wxLIST_FORMAT_LEFT, 100);

  m_btnAddDir->Hide();
#if !NDEBUG
  SetTitle(GetTitle() + "  [" + _("DEBUG Build") + "]");
#endif
}

void MainAppFrame::ApplyConfigFromStore() {
  using namespace umd::config;
  using namespace umd::utils;
  using namespace umc;

  auto& config = AppConfigStore::GetInstance()->GetLoadedConfig();
  kugou::KGMMaskGenerator::GetInstance()->SetTable(
      config.kugou.t1, config.kugou.t2, config.kugou.v2);

  ximalaya::X2MCipher::SetContentKey(config.xmly.x2m_content_key);
  ximalaya::X3MCipher::SetContentKey(config.xmly.x3m_content_key);
  ximalaya::X3MCipher::SetScrambleTable(config.xmly.x3m_scramble_indexes);

  tencent::StaticStreamCipher::SetStaticKey(config.tencent.static_key);
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
  if (optionsDialog->IsConfigSaved()) {
    const auto& config = optionsDialog->GetSavedConfig();
    const auto config_store = umd::config::AppConfigStore::GetInstance();
    config_store->UpdateConfig(config);

    if (config_store->SaveConfigToDisk()) {
      ApplyConfigFromStore();
      wxMessageBox(_("Config saved. Some config may require restart to work."),
                   LOCALISED_APP_NAME);
    } else {
      wxMessageBox(_("Could not save config."), LOCALISED_APP_NAME,
                   wxICON_ERROR);
    }
  }
  optionsDialog->Destroy();
}

void MainAppFrame::SetDecryptionInProgress(bool in_progress) {
  std::lock_guard<std::mutex> guard(update_status_mutex_);
  m_btnClearLogs->Enable(!in_progress);
  m_btnProcess->Enable(!in_progress);
  m_btnOptions->Enable(!in_progress);
}

#define QMCv1_FILTER "*.qmcflac;*.qmcflac0;*.qmc;*.qmc0;"
#define QMCv2_FILTER "*.mgg;*.mgg0;*.mgg1;*.mflac;*.mflac0;*.mflac1"
#define Kugou_FILTER "*.kgm;*.vpr"
#define Kuwo_FILTER "*.kwm"
#define Ximalaya_FILTER "*.x2m;*.x3m"
#define Xiami_FILTER "*.xm"
#define ALL_SUPPORTED_FILTER                                     \
  QMCv1_FILTER ";" QMCv2_FILTER ";" Kugou_FILTER ";" Kuwo_FILTER \
               ";" Ximalaya_FILTER ";" Xiami_FILTER

void MainAppFrame::OnButtonClick_AddFile(wxCommandEvent& event) {
  event.Skip();

  wxString filter;
  filter += _("All supported files");
  filter += wxT("|" ALL_SUPPORTED_FILTER "|");
  filter += _("QMCv1 files (*.qmcflac;*.qmc)");
  filter += wxT("|" QMCv1_FILTER "|");
  filter += _("QMCv2 files (*.mgg;*.mflac)");
  filter += wxT("|" QMCv2_FILTER "|");
  filter += _("Kugou music (*.kgm;*.vpr)");
  filter += wxT("|" Kugou_FILTER "|");
  filter += _("Kuwo music (*.kwm)");
  filter += wxT("|" Kuwo_FILTER "|");
  filter += _("Ximalaya audio file (*.x2m; *.x3m)");
  filter += wxT("|" Ximalaya_FILTER "|");
  filter += _("Xiami music (*.xm)");
  filter += wxT("|" Xiami_FILTER "|");
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

void MainAppFrame::HandleAddFilesToQueue(const wxArrayString& file_paths) {
  auto len = file_paths.GetCount();
  for (int i = 0; i < len; i++) {
    umc::Path item_path(umc::U8StrFromStr(file_paths.Item(i).utf8_string()));

    wxListItem new_item;
    new_item.SetText(wxT(""));
    new_item.SetId(m_decryptLogs->GetItemCount());

    auto rowIndex = m_decryptLogs->InsertItem(new_item);
    file_entries_.push_back(std::make_shared<FileEntry>(FileEntry{
        FileProcessStatus::kNotProcessed,
        item_path,
        rowIndex,
        0,
    }));
    UpdateFileStatus(rowIndex, FileProcessStatus::kNotProcessed);
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
#if __UMD_SINGLE_THREAD_MODE
    this->ProcessNextFile();
#else
    umd::io_service.post([this]() { this->ProcessNextFile(); });
#endif
  }
}

void MainAppFrame::UpdateFileStatus(int idx, FileProcessStatus status) {
  std::lock_guard<std::mutex> guard(update_status_mutex_);

  auto entry = file_entries_.at(idx);
  entry->status = status;
  auto name_u8 = entry->file_path.filename().u8string();
  auto name = wxString::FromUTF8(reinterpret_cast<const char*>(name_u8.data()));

  wxString status_text;
  switch (status) {
    case FileProcessStatus::kNotProcessed:
      status_text.Printf(_("Ready: %s"), name);
      break;
    case FileProcessStatus::kProcessedOk:
      status_text.Printf(_("Decode OK: %s (%lums)"), name,
                         entry->process_time_ms);
      break;
    case FileProcessStatus::kProcessFailed:
      status_text.Printf(_("FAIL: %s (%s)"), name, entry->error);
      break;
    case FileProcessStatus::kProcessing:
      status_text.Printf(_("Processing: %s"), name);
      break;
    case FileProcessStatus::kProcessNotSupported:
      status_text.Printf(_("Unsupported: %s"), name);
      break;

    default:
      status_text.Printf(_("Unknown state (code %d): %s"), (int)status, name);
      break;
  }

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

  main_thread_runner_.PostInMainThread([this, current_index]() {
    UpdateFileStatus(current_index, FileProcessStatus::kProcessing);
  });

  auto decryptor = std::make_unique<umd::utils::AudioDecryptorManager>();
  decryptor->Open(entry->file_path);

  system_clock::time_point time_before_process = system_clock::now();
  FileProcessStatus status = FileProcessStatus::kProcessNotSupported;

  auto encryption = decryptor->SniffEncryption();
  if (encryption == EncryptionType::kNotEncrypted ||
      encryption == EncryptionType::kUnsupported) {
    status = FileProcessStatus::kProcessNotSupported;
  } else {
    // TODO: show encryption type
    if (decryptor->DecryptAudioFile()) {
      status = FileProcessStatus::kProcessedOk;
    } else {
      status = FileProcessStatus::kProcessFailed;
    }
  }

  system_clock::time_point time_after_process = system_clock::now();
  auto t = boost::chrono::duration_cast<boost::chrono::milliseconds>(
      time_after_process - time_before_process);
  entry->process_time_ms = static_cast<long>(t.count());
  entry->error = decryptor->GetError();

  main_thread_runner_.PostInMainThread([this, current_index, status]() {
    UpdateFileStatus(current_index, status);

    OnProcessSingleFileComplete();
  });
}

void MainAppFrame::OnProcessSingleFileComplete() {
  int completion_count = file_entry_complete_count_.fetch_add(1) + 1;
  if (completion_count == file_entries_.size()) {
    SetDecryptionInProgress(false);
  }
}
