#pragma once

#include <filesystem>

#if !__APPLE__ && !_WIN32

bool GetDataDirectoryImpl(std::filesystem::path &data_dir)
{
    return false;
}

#endif
