find_path(BOTAN_INCLUDE_DIRS NAMES botan/botan.h)

get_filename_component(_prefix_path ${BOTAN_INCLUDE_DIRS} PATH)

find_library(
    BOTAN_LIBRARY_DEBUG
    NAMES botan-static botan botan-2-static botan-2 botan2-static botan2
    PATHS ${_prefix_path}/debug/lib
    NO_DEFAULT_PATH
)
find_library(
    BOTAN_LIBRARY_RELEASE
    NAMES botan-static botan botan-2-static botan-2 botan2-static botan2
    PATHS ${_prefix_path}/lib
    NO_DEFAULT_PATH
)

unset(_prefix_path)

include(SelectLibraryConfigurations)
select_library_configurations(BOTAN)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Botan
    REQUIRED_VARS BOTAN_LIBRARIES BOTAN_INCLUDE_DIRS
)
