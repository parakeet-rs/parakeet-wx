#include "app.h"
#include "config/AppConfigStore.h"
#include "dialog/MainAppFrame.h"
#include "utils/AppDataPath.h"
#include "utils/threading.h"

#include <wx/stdpaths.h>

#include <memory>

IMPLEMENT_APP(umDesktopApp)

std::unique_ptr<wxLocale> locale;

void InitLocale(const std::string& str_name) {
  long lang_code = wxLANGUAGE_DEFAULT;

  auto lang_info = wxLocale::FindLanguageInfo(wxString(str_name));

  if (lang_info) {
    std::cout << "using language from config - " << str_name << std::endl;
    lang_code = lang_info->Language;
  }
  if (!wxLocale::IsAvailable(lang_code)) lang_code = wxLANGUAGE_DEFAULT;

  locale = std::make_unique<wxLocale>(lang_code);

  // Read from executable directory / AppImage directory
  locale->AddCatalogLookupPathPrefix(
      wxString(umd::utils::GetExecutableDirectory().c_str()));
  locale->AddCatalogLookupPathPrefix(
      wxString(umd::utils::GetAppImageDirOrExeDirectory().c_str()));

#ifdef __WXGTK__
  // add locale search paths
  locale->AddCatalogLookupPathPrefix(wxT("/usr"));
  locale->AddCatalogLookupPathPrefix(wxT("/usr/local"));
  wxStandardPaths* paths = (wxStandardPaths*)&wxStandardPaths::Get();
  wxString prefix = paths->GetInstallPrefix();
  locale->AddCatalogLookupPathPrefix(prefix);
#endif

  locale->AddCatalog(wxT("um-desktop"));

  if (!locale->IsOk()) {
    std::cerr << "load language failed, reset to default." << std::endl;
    locale = std::make_unique<wxLocale>(wxLANGUAGE_DEFAULT);
  }
}

bool umDesktopApp::OnInit() {
  auto config_store = umd::config::AppConfigStore::GetInstance();
  config_store->LoadConfigFromDisk();
  auto& general_config = config_store->GetLoadedConfig().desktop.general;

  umd::io_service_start(general_config.thread_count);

  InitLocale(general_config.locale);
  wxFrame* frame = new MainAppFrame(nullptr);

  frame->Show(true);
  SetTopWindow(frame);
  return true;
}

int umDesktopApp::OnExit() {
  umd::io_service_stop();
  return 0;
}
