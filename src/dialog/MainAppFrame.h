#pragma once
#include "config/AppConfigStore.h"
#include "parakeet-crypto/ITransformer.h"
#include "ui/ui.h"
#include "utils/wxMainThreadRunner.hpp"

#include <optional>
#include <wx/dnd.h>

#include <atomic>
#include <fstream>
#include <memory>
#include <mutex>
#include <vector>

enum class FileProcessStatus
{
    kNotProcessed = 0,
    kProcessing,
    kProcessedOk,
    kProcessFailed,
    kProcessNotEncrypted,
    kProcessNotSupported,
};

struct FileEntry
{
    FileProcessStatus status;
    std::filesystem::path file_path;
    long index;
    long process_time_ms;
    wxString error;
    std::shared_ptr<parakeet_crypto::ITransformer> transformer;
    const char *ext;
};

class MainAppFrame;

class MainAppDropTarget : public wxFileDropTarget
{
  public:
    MainAppDropTarget(MainAppFrame *app_frame) : wxFileDropTarget(), app_frame_(app_frame)
    {
    }
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames) override;

  private:
    MainAppFrame *app_frame_;
};

class MainAppFrame : public parakeet_wx::ui_base::uiMainAppFrame
{
  public:
    MainAppFrame(wxWindow *parent, wxWindowID id = wxID_ANY);
    virtual ~MainAppFrame();

    void SetDecryptionInProgress(bool in_progress);

  protected:
    std::atomic<unsigned int> file_entry_process_idx_ = 0;
    std::atomic<unsigned int> file_entry_complete_count_ = 0;
    std::vector<std::shared_ptr<FileEntry>> file_entries_;

    void uiMainAppFrameOnSize(wxSizeEvent &event) override;
    void OnBtnClickOptions(wxCommandEvent &event) override;
    void OnButtonClick_AddFile(wxCommandEvent &event) override;
    void OnButtonClick_AddDirectory(wxCommandEvent &event) override;
    void OnButtonClick_ClearLogs(wxCommandEvent &event) override;
    void OnButtonClick_ProcessFiles(wxCommandEvent &event) override;
    void OnThreadEvent(wxThreadEvent &event);

    std::mutex update_status_mutex_;
    void UpdateFileStatus(int idx, FileProcessStatus status);
    void ProcessNextFile();
    void OnProcessSingleFileComplete();
    void HandleAddFilesToQueue(const wxArrayString &filenames);
    void AddSingleFileToQueue(const std::filesystem::path &path);

  private:
    friend class MainAppDropTarget;
    parakeet_wx::utils::wxMainThreadRunner main_thread_runner_;
};
