#pragma once

#include <filesystem>

namespace parakeet_wx::utils
{

const std::filesystem::path &GetExecutableDirectory();
const std::filesystem::path &GetUserDataDirectory();
const std::filesystem::path &GetAppImageDirOrExeDirectory();

} // namespace parakeet_wx::utils
