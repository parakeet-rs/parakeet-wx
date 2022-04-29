cmake_minimum_required(VERSION 3.15)
project(um-desktop)

if(MSVC)
  # Force static link
  cmake_policy(SET CMP0091 NEW)
  set(MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

  add_compile_options(
    $<$<CONFIG:>:/MT> #---------|
    $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
    $<$<CONFIG:Release>:/MT> #--|
  )

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")  
endif()
set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets REQUIRED)
if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("HACK: Replace wxWidgets with debug-static path")
  list(TRANSFORM wxWidgets_LIBRARIES REPLACE "lib/(wx[a-zA-Z]+[0-9]*u?)_([a-zA-Z0-9]+).lib" "debug/lib/\\1d_\\2.lib")
  list(TRANSFORM wxWidgets_LIBRARIES REPLACE "lib/(wx[a-zA-Z]+[0-9]*u?).lib" "debug/lib/\\1d.lib")
  message("wxWidgets_LIBRARIES: ${wxWidgets_LIBRARIES}")
endif()

file(GLOB SOURCES src/*.cpp src/**/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${wxWidgets_DEFINITIONS}
  "$<$<CONFIG:DEBUG>:${wxWidgets_DEFINITIONS_DEBUG}>"
)
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

# i18n text
file(GLOB MY_I18N_FILE
  "locale/*"
)
file(
  COPY ${MY_I18N_FILE} DESTINATION ./
  PATTERN "*.po" EXCLUDE
  PATTERN "*.pot" EXCLUDE
)
