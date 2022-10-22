#include "app_version.h"

#if __has_include("constants.build_time.h")
#include "constants.build_time.h"
#endif

#ifndef UM_DESKTOP_VERSION_STR
#define UM_DESKTOP_VERSION_STR _("Unknown Version")
#endif

const char* const umd_version = UM_DESKTOP_VERSION_STR;
