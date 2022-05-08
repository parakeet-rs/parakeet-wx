#include "app.h"
#include "config/AppConfigStore.h"
#include "dialog/MainAppFrame.h"
#include "utils/threading.h"

#include <wx/stdpaths.h>

IMPLEMENT_APP(umDesktopApp)

wxLocale* locale;
long language;

void initLanguageSupport() {
  language = wxLANGUAGE_DEFAULT;

  // fake functions, use proper implementation
#if _WIN32
  language = wxLANGUAGE_CHINESE_SIMPLIFIED;
#endif

  // load language if possible, fall back to english otherwise
  if (wxLocale::IsAvailable(language)) {
    locale = new wxLocale(language);

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
      std::cerr << "selected language is wrong" << std::endl;
      delete locale;
      locale = new wxLocale(wxLANGUAGE_ENGLISH);
      language = wxLANGUAGE_ENGLISH;
    }
  } else {
    std::cout << "The selected language is not supported by your system."
              << "Try installing support for this language." << std::endl;
    locale = new wxLocale(wxLANGUAGE_ENGLISH);
    language = wxLANGUAGE_ENGLISH;
  }
}

bool umDesktopApp::OnInit() {
  auto config_store = umd::config::AppConfigStore::GetInstance();
  config_store->LoadConfigFromDisk();

  umd::io_service_start();

  initLanguageSupport();
  wxFrame* frame = new MainAppFrame(nullptr);

  frame->Show(true);
  SetTopWindow(frame);
  return true;
}

int umDesktopApp::OnExit() {
  umd::io_service_stop();
  return 0;
}
