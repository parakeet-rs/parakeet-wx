cmake_minimum_required(VERSION 3.15)
project(um-root)

# Add our modules...
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

if(MSVC)
  # Force static link
  cmake_policy(SET CMP0091 NEW)
  set(MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

  add_compile_options(
    $<$<CONFIG:>:/MT> #---------|
    $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
    $<$<CONFIG:Release>:/MT> #--|
  )
else()
  add_compile_options($<$<CONFIG:RELEASE>:-s>)
  add_link_options($<$<CONFIG:RELEASE>:-s>)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  add_compile_options(-DNDEBUG=1 -D_NDEBUG=1)
endif()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(um-crypto)
add_subdirectory(um-desktop)

# i18n text
file(GLOB MY_I18N_FILE
  "locale/*"
)
file(
  COPY ${MY_I18N_FILE} DESTINATION ./
  PATTERN "*.po" EXCLUDE
)
