#pragma once

#include <filesystem>

namespace umd::utils {

const std::filesystem::path& GetExecutableDirectory();
const std::filesystem::path& GetUserDataDirectory();
const std::filesystem::path& GetAppImageDirOrExeDirectory();

}  // namespace umd::utils
