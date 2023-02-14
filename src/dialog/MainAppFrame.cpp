#include "MainAppFrame.h"
#include "OptionsDialog.h"

#include "constants.h"
#include "res/parakeet.xpm"
#include "utils/MakeArray.h"
#include "utils/threading.h"
#include "utils/transform_error_text.h"

#include <parakeet-crypto/StreamHelper.h>

#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/propgrid/propgrid.h>
#include <wx/wfstream.h>

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

#include <algorithm>
#include <fstream>
#include <functional>
#include <string>
#include <thread>

using boost::chrono::system_clock;

bool MainAppDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &file_paths)
{
#if defined(PARAKEET_SINGLE_THREADED)
    app_frame_->HandleAddFilesToQueue(file_paths);
#else
    parakeet_wx::io_service.post([this, file_paths]() { app_frame_->HandleAddFilesToQueue(file_paths); });
#endif

    return true;
}

void MainAppFrame::OnThreadEvent(wxThreadEvent &event)
{
    if (!main_thread_runner_.HandleMainThreadEvent(event))
    {
        event.Skip();
    }
}

MainAppFrame::~MainAppFrame()
{
    Unbind(wxEVT_THREAD, &MainAppFrame::OnThreadEvent, this);
}

MainAppFrame::MainAppFrame(wxWindow *parent, wxWindowID id) : uiMainAppFrame(parent, id)
{
    SetIcon(wxICON(appicon));

    // Rescale for HiDPI support
    SetClientSize(FromDIP(GetClientSize()));
    Center();

    // Bootstrap Multi-thread handling
    main_thread_runner_.SetMainThreadRunnerEventHandler(GetEventHandler());
    Bind(wxEVT_THREAD, &MainAppFrame::OnThreadEvent, this);

    // Bootstrap drag & drop
    SetDropTarget(new MainAppDropTarget(this));

    // Setup decryptions logs
    m_decryptLogs->InsertColumn(0, wxT(""), wxLIST_FORMAT_LEFT, 100);

    m_btnAddDir->Hide();
#if !NDEBUG
    SetTitle(GetTitle() + "  [" + _("DEBUG Build") + "]");
#endif

    SetTitle(GetTitle() + "  (" + APP_VERSION + (PARAKEET_GIT_TREE_DIRTY ? "*" : "") + ")");
}

void MainAppFrame::uiMainAppFrameOnSize(wxSizeEvent &event)
{
    event.Skip();

    int w, h;
    this->m_decryptLogs->GetSize(&w, &h);
    this->m_decryptLogs->SetColumnWidth(0, w);
};

void MainAppFrame::OnBtnClickOptions(wxCommandEvent &event)
{
    auto optionsDialog = new OptionsDialog(this);
    optionsDialog->ShowModal();
    if (optionsDialog->IsConfigSaved())
    {
        const auto &config = optionsDialog->GetSavedConfig();
        const auto config_store = parakeet_wx::config::AppConfigStore::GetInstance();
        config_store->UpdateConfig(config);

        if (config_store->SaveConfigToDisk())
        {
            wxMessageBox(_("Config saved. Some config may require restart to work."), LOCALISED_APP_NAME);
        }
        else
        {
            wxMessageBox(_("Could not save config."), LOCALISED_APP_NAME, wxICON_ERROR);
        }
    }
    optionsDialog->Destroy();
}

void MainAppFrame::SetDecryptionInProgress(bool in_progress)
{
    std::lock_guard<std::mutex> guard(update_status_mutex_);
    m_btnClearLogs->Enable(!in_progress);
    m_btnProcess->Enable(!in_progress);
    m_btnOptions->Enable(!in_progress);
}

#define QMC_WEYUN_FILTER "*.666c6163;*.776176;*.6d7033;*.6f6767;*.6d3461"
#define QMCv1_FILTER                                                                                                   \
    "*.qmcflac;*.qmcflac?;*.qmc;*.qmc?"                                                                                \
    ";" QMC_WEYUN_FILTER
#define QMCv2_FILTER "*.mgg;*.mgg?;*.mflac;*.mflac?;*.mflac1"
#define JOOX_FILTER "*.ofl_en"
#define Kugou_FILTER "*.kgm;*.vpr"
#define Kuwo_FILTER "*.kwm"
#define Netease_FILTER "*.ncm"
#define Ximalaya_FILTER "*.x2m;*.x3m"
#define Xiami_FILTER "*.xm"
#define ADD_FILTER_EXT(FILTER) filter += wxT(" (" FILTER ")|" FILTER "|")
#define ALL_SUPPORTED_FILTER                                                                                           \
    QMCv1_FILTER ";" QMCv2_FILTER ";" JOOX_FILTER ";" Kugou_FILTER ";" Kuwo_FILTER ";" Netease_FILTER                  \
                 ";" Ximalaya_FILTER ";" Xiami_FILTER

void MainAppFrame::OnButtonClick_AddFile(wxCommandEvent &event)
{
    event.Skip();

    static wxString filter = []() -> wxString {
        wxString filter;
        filter += _("All known extensions files");
        filter += wxT("|" ALL_SUPPORTED_FILTER "|");
        filter += _("Koox music");
        ADD_FILTER_EXT(JOOX_FILTER);
        filter += _("QMCv1 files");
        ADD_FILTER_EXT(QMCv1_FILTER);
        filter += _("QMCv2 files");
        ADD_FILTER_EXT(QMCv2_FILTER);
        filter += _("Kugou music");
        ADD_FILTER_EXT(Kugou_FILTER);
        filter += _("Kuwo music");
        ADD_FILTER_EXT(Kuwo_FILTER);
        filter += _("Netease music");
        ADD_FILTER_EXT(Netease_FILTER);
        filter += _("Ximalaya audio file");
        ADD_FILTER_EXT(Ximalaya_FILTER);
        filter += _("Xiami music");
        ADD_FILTER_EXT(Xiami_FILTER);
        filter += _("All files (*.*)|*");
        return filter;
    }();

    wxFileDialog openFileDialog(this, _("Open encrypted music files"), "", "", filter,
                                wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // the user changed idea...

    wxArrayString paths;
    openFileDialog.GetPaths(paths);

#if defined(PARAKEET_SINGLE_THREADED)
    HandleAddFilesToQueue(paths);
#else
    parakeet_wx::io_service.post([this, paths]() { HandleAddFilesToQueue(paths); });
#endif
}

void MainAppFrame::HandleAddFilesToQueue(const wxArrayString &file_paths)
{
    auto len = file_paths.GetCount();

    for (int i = 0; i < len; i++)
    {
        auto path_item = file_paths.Item(i);
#if _WIN32
        std::filesystem::path item_path(path_item.wchar_str().data());
#else
        std::filesystem::path item_path(path_item.utf8_str().data());
#endif
        AddSingleFileToQueue(item_path);
    }
}

void MainAppFrame::AddSingleFileToQueue(const std::filesystem::path &path)
{
    namespace fs = std::filesystem;
    auto status = fs::status(path);
    if (fs::is_directory(path))
    {
        for (const auto &entry : fs::directory_iterator{path})
        {
            AddSingleFileToQueue(entry.path());
        }
        return;
    }
    else if (!fs::is_regular_file(path))
    {
        return;
    }

    auto decryption_manager = parakeet_wx::config::AppConfigStore::GetInstance()->GetDecryptionManager();
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs.is_open())
    {
        wxListItem new_item;
        new_item.SetText(wxT(""));
        new_item.SetId(m_decryptLogs->GetItemCount());

        auto rowIndex = m_decryptLogs->InsertItem(new_item);
        file_entries_.push_back(std::make_shared<FileEntry>(FileEntry{
            /* .status =          */ FileProcessStatus::kProcessFailed,
            /* .file_path =       */ {},
            /* .index =           */ rowIndex,
            /* .process_time_ms = */ 0,
            /* .error =           */ wxT(""),
            /* .transformer =     */ nullptr,
            /* .ext =             */ "",
        }));
        UpdateFileStatus(rowIndex, FileProcessStatus::kProcessFailed);
        return;
    }
    auto transformer_result = decryption_manager->FindDecryptionTransformer(ifs);

    this->main_thread_runner_.PostInMainThread([this, transformer_result, path]() {
        auto supported = transformer_result.has_value();

        wxListItem new_item;
        new_item.SetText(wxT(""));
        new_item.SetId(m_decryptLogs->GetItemCount());

        auto rowIndex = m_decryptLogs->InsertItem(new_item);
        file_entries_.push_back(std::make_shared<FileEntry>(FileEntry{
            /* .status =          */ FileProcessStatus::kNotProcessed,
            /* .file_path =       */ path,
            /* .index =           */ rowIndex,
            /* .process_time_ms = */ 0,
            /* .error =           */ wxT(""),
            /* .transformer =     */ transformer_result ? transformer_result->transformer : nullptr,
            /* .ext =             */ transformer_result ? transformer_result->ext : "",
        }));
        UpdateFileStatus(rowIndex,
                         supported ? FileProcessStatus::kNotProcessed : FileProcessStatus::kProcessNotSupported);
    });
}

void MainAppFrame::OnButtonClick_AddDirectory(wxCommandEvent &event)
{
    event.Skip();
}

void MainAppFrame::OnButtonClick_ClearLogs(wxCommandEvent &event)
{
    for (int i = file_entries_.size() - 1; i >= 0; i--)
    {
        m_decryptLogs->DeleteItem(0);
    }

    file_entries_.clear();
    file_entry_process_idx_.store(0);
    file_entry_complete_count_.store(0);
}

void MainAppFrame::OnButtonClick_ProcessFiles(wxCommandEvent &event)
{
    SetDecryptionInProgress(true);

    const int len = file_entries_.size() - file_entry_complete_count_;
    if (len == 0)
    {
        SetDecryptionInProgress(false);
        return;
    }

    for (int i = len - 1; i >= 0; i--)
    {
#if defined(PARAKEET_SINGLE_THREADED)
        this->ProcessNextFile();
#else
        parakeet_wx::io_service.post([this]() { this->ProcessNextFile(); });
#endif
    }
}

void MainAppFrame::UpdateFileStatus(int idx, FileProcessStatus status)
{
    std::lock_guard<std::mutex> guard(update_status_mutex_);

    auto entry = file_entries_.at(idx);
    entry->status = status;
    auto name_u8 = entry->file_path.filename().u8string();
    auto name = wxString::FromUTF8(reinterpret_cast<const char *>(name_u8.data()));

    wxString encrypted_type = "unknown";
    wxString audio_ext = "unknown";
    if (entry->transformer)
    {
        encrypted_type = entry->transformer->GetName(); // TODO: Get type name?
        audio_ext = entry->ext;
    }

    wxString status_text;
    switch (status)
    {
    case FileProcessStatus::kNotProcessed:
        status_text.Printf(_("Ready [%s -> %s]: %s"), encrypted_type, audio_ext, name);
        break;
    case FileProcessStatus::kProcessedOk:
        status_text.Printf(_("Decode OK [%s -> %s]: %s (%lums)"), encrypted_type, audio_ext, name,
                           entry->process_time_ms);
        break;
    case FileProcessStatus::kProcessFailed:
        status_text.Printf(_("FAIL [%s]: %s (%s)"), encrypted_type, name, entry->error);
        break;
    case FileProcessStatus::kProcessing:
        status_text.Printf(_("Converting from %s to %s: %s"), encrypted_type, audio_ext, name);
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

// using parakeet_wx::utils::EncryptionType;

void MainAppFrame::ProcessNextFile()
{
    auto current_index = file_entry_process_idx_.fetch_add(1);
    if (current_index >= file_entries_.size())
    {
        file_entry_process_idx_.fetch_sub(1);
        return;
    }

    auto entry = file_entries_.at(current_index);

    if (entry->status != FileProcessStatus::kNotProcessed)
    {
        main_thread_runner_.PostInMainThread([this]() { OnProcessSingleFileComplete(); });
        return;
    }

    system_clock::time_point time_before_process = system_clock::now();
    FileProcessStatus status = FileProcessStatus::kProcessNotSupported;

    using parakeet_crypto::InputFileStream;
    using parakeet_crypto::OutputFileStream;
    using parakeet_crypto::TransformResult;

    bool ok = false;

    auto process_entry = [&]() {
        auto path_out = std::filesystem::path(entry->file_path).replace_extension(entry->ext);
        std::ifstream ifs(entry->file_path, std::ios::binary);

        if (!ifs.is_open())
        {
            entry->error = _("could not open input file");
            return;
        }

        std::ofstream ofs(path_out, std::ios::binary);
        InputFileStream reader{ifs};
        OutputFileStream writer{ofs};
        reader.Seek(0, parakeet_crypto::SeekDirection::SEEK_FILE_BEGIN);
        auto transform_result = entry->transformer->Transform(&writer, &reader);
        ok = transform_result == TransformResult::OK;
        if (!ok)
        {
            entry->error = parakeet_wx::to_string(transform_result);
        }
    };

    if (entry->transformer)
    {
        main_thread_runner_.PostInMainThread(
            [this, current_index]() { UpdateFileStatus(current_index, FileProcessStatus::kProcessing); });
        process_entry();
    }

    if (ok)
    {
        status = FileProcessStatus::kProcessedOk;
    }
    else
    {
        status = FileProcessStatus::kProcessFailed;
    }

    system_clock::time_point time_after_process = system_clock::now();
    auto t = boost::chrono::duration_cast<boost::chrono::milliseconds>(time_after_process - time_before_process);
    entry->process_time_ms = static_cast<long>(t.count());

    main_thread_runner_.PostInMainThread([this, current_index, status]() {
        UpdateFileStatus(current_index, status);

        OnProcessSingleFileComplete();
    });
}

void MainAppFrame::OnProcessSingleFileComplete()
{
    int completion_count = file_entry_complete_count_.fetch_add(1) + 1;
    if (completion_count == file_entries_.size())
    {
        SetDecryptionInProgress(false);
    }
}
