#pragma once

#if __APPLE__

bool GetDataDirectoryImpl(std::filesystem::path &data_dir)
{
    return false;
}

#endif
