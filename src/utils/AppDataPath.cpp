#include "constants.h"

#include "AppDataPath.h"
// Example implementation taken from:
//   https://github.com/Malvineous/cfgpath/blob/16b438c/cfgpath.h#L185-L187
//  *   Windows: C:\Users\jcitizen\AppData\Roaming\appname\
//  *   Linux: /home/jcitizen/.config/appname/
//  *   Mac: /Users/jcitizen/Library/Application Support/appname/

#include "AppDataPath/apple_impl.hpp"
#include "AppDataPath/linux_impl.hpp"
#include "AppDataPath/windows_impl.hpp"

#include <boost/dll/runtime_symbol_info.hpp>
#include <wx/config.h>

#include <cstddef>
#include <filesystem>

namespace fs = std::filesystem;

namespace parakeet_wx::utils
{

inline fs::path GetExecutablePath()
{
    return fs::path(boost::dll::program_location().native());
}

inline fs::path GetAppImagePathOrExePath()
{
    if (const char *env_AppImagePath = std::getenv("APPIMAGE"))
    {
        return fs::path(env_AppImagePath);
    }

    return GetExecutablePath();
}

inline fs::path GetLatestUserDataDirectory()
{
    // Check if we are running portable mode
    auto exe_portable_folder = GetAppImageDirOrExeDirectory() / APP_INTERNAL_NAME;
    if (fs::is_directory(exe_portable_folder))
    {
        return exe_portable_folder;
    }

    // Get platform specific implementation;
    //   fallback to the "portable" directory.
    fs::path user_app_data;
    if (!GetDataDirectoryImpl(user_app_data))
    {
        user_app_data = exe_portable_folder;
    }

    std::error_code ec;
    fs::create_directories(user_app_data, ec);
    if (ec)
    {
        throw std::runtime_error("could not find usable appdir.");
    }

    return user_app_data;
}

// return a cached copy instead.
const std::filesystem::path &GetExecutableDirectory()
{
    static std::filesystem::path exe_dir = GetExecutablePath().parent_path();

    return exe_dir;
}

const std::filesystem::path &GetAppImageDirOrExeDirectory()
{
    static std::filesystem::path exe_dir = GetAppImagePathOrExePath().parent_path();

    return exe_dir;
}

// return a cached copy instead.
const std::filesystem::path &GetUserDataDirectory()
{
    static std::filesystem::path user_dir = GetLatestUserDataDirectory();

    return user_dir;
}

} // namespace parakeet_wx::utils
