find_path(BOTAN_INCLUDE_DIRS NAMES botan/botan.h)

# botan.lib
find_library(BOTAN_LIBRARIES NAMES botan)

# Use some standard module to handle the QUIETLY and REQUIRED arguments, and
# set BOTAN_FOUND to TRUE if these two variables are set.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BOTAN REQUIRED_VARS BOTAN_LIBRARIES BOTAN_INCLUDE_DIRS)

if(BOTAN_FOUND)
  set(BOTAN_LIBRARY ${BOTAN_LIBRARIES})
  set(BOTAN_INCLUDE_DIR ${BOTAN_INCLUDE_DIRS})
endif()

mark_as_advanced(BOTAN_INCLUDE_DIRS BOTAN_LIBRARIES)
