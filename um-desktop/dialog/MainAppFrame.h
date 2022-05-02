#pragma once
#include <boost/asio/io_service.hpp>
#include <boost/asio/thread_pool.hpp>
#include "../ui/ui.h"

#include <memory>
#include <mutex>
#include <vector>

enum class FileProcessStatus {
  kNotProcessed = 0,
  kProcessing,
  kProcessedOk,
  kProcessFailed,
  kProcessNotEncrypted,
  kProcessNotSupported,
};

struct FileEntry {
  FileProcessStatus status;
  wxString file_path;
  int index;
};

class MainAppFrame : public umd::ui_base::uiMainAppFrame {
 public:
  MainAppFrame(wxWindow* parent, wxWindowID id = wxID_ANY);
  void SetInProcess(bool in_process);

 protected:
  std::atomic<unsigned int> file_entry_process_idx_ = 0;
  std::atomic<unsigned int> file_entry_complete_count_ = 0;
  std::vector<std::shared_ptr<FileEntry>> file_entries_;

  void uiMainAppFrameOnSize(wxSizeEvent& event) override;
  void OnBtnClickOptions(wxCommandEvent& event) override;
  void OnButtonClick_AddFile(wxCommandEvent& event) override;
  void OnButtonClick_AddDirectory(wxCommandEvent& event) override;
  void OnButtonClick_ClearLogs(wxCommandEvent& event) override;
  void OnButtonClick_ProcessFiles(wxCommandEvent& event) override;

  std::mutex update_status_mutex_;
  void UpdateFileStatus(int idx, FileProcessStatus status);
  void ProcessNextFile();
  void OnProcessSingleFileComplete();
};
