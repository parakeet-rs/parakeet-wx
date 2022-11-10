#pragma once

#if _WIN32
#include <ShlObj_core.h>
#include <filesystem>

bool GetDataDirectoryImpl(std::filesystem::path& data_dir) {
  wchar_t* p_data_dir_unicode;
  const auto err = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, NULL, &p_data_dir_unicode);
  data_dir = std::filesystem::path(p_data_dir_unicode) / APP_INTERNAL_NAME;

  // See doc:
  //   https://docs.microsoft.com/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath
  // > The calling process is responsible for freeing this resource ...
  // whether >   SHGetKnownFolderPath succeeds or not
  CoTaskMemFree(p_data_dir_unicode);

  return S_OK == err;
}

#endif
