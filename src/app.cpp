#include "app.h"
#include "config/AppConfigStore.h"
#include "dialog/MainAppFrame.h"
#include "utils/AppDataPath.h"
#include "utils/threading.h"

#include <wx/stdpaths.h>

#include <clocale>
#include <memory>

IMPLEMENT_APP(ParakeetWx)

wxLocale *locale = nullptr;

void InitLocale(const std::string &str_name)
{
    long lang_code = wxLANGUAGE_DEFAULT;

    auto lang_info = wxLocale::FindLanguageInfo(wxString(str_name));

    if (lang_info)
    {
        std::cout << "using language from config - " << str_name << std::endl;
        lang_code = lang_info->Language;
    }
    if (!wxLocale::IsAvailable(lang_code))
        lang_code = wxLANGUAGE_DEFAULT;

    locale = new wxLocale(lang_code);

    locale->AddCatalogLookupPathPrefix(wxString(parakeet_wx::utils::GetExecutableDirectory()));
    locale->AddCatalogLookupPathPrefix(wxString(parakeet_wx::utils::GetAppImageDirOrExeDirectory()));

#ifdef __WXGTK__
    // add locale search paths
    locale->AddCatalogLookupPathPrefix(wxT("/usr"));
    locale->AddCatalogLookupPathPrefix(wxT("/usr/local"));
    wxStandardPaths *paths = (wxStandardPaths *)&wxStandardPaths::Get();
    wxString prefix = paths->GetInstallPrefix();
    locale->AddCatalogLookupPathPrefix(prefix);
#endif

    locale->AddCatalog(wxT("um-desktop"));

    if (!locale->IsOk())
    {
        std::cerr << "load language failed, reset to default." << std::endl;
        delete locale;
        locale = new wxLocale(wxLANGUAGE_DEFAULT);
    }
}

bool ParakeetWx::OnInit()
{
#if _WIN32
    setlocale(LC_ALL, ".65001");
#endif

    auto config_store = parakeet_wx::config::AppConfigStore::GetInstance();
    config_store->LoadConfigFromDisk();
    auto &general_config = config_store->GetLoadedConfig().desktop.general;

    parakeet_wx::io_service_start(general_config.thread_count);

    InitLocale(general_config.locale);
    wxFrame *frame = new MainAppFrame(nullptr);

    frame->Show(true);
    SetTopWindow(frame);
    return true;
}

int ParakeetWx::OnExit()
{
    parakeet_wx::io_service_stop();
    return 0;
}
