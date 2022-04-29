cmake_minimum_required(VERSION 3.14)
project(um-desktop)

if(MSVC)
  add_compile_options(
    $<$<CONFIG:>:/MT> #---------|
    $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
    $<$<CONFIG:Release>:/MT> #--|
  )
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

file(GLOB SOURCES src/*.cpp src/**/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${wxWidgets_DEFINITIONS}
  "$<$<CONFIG:DEBUG>:${wxWidgets_DEFINITIONS_DEBUG}>"
)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})

# i18n text
file(GLOB MY_I18N_FILE
  "locale/*"
)
file(
  COPY ${MY_I18N_FILE} DESTINATION ./
  PATTERN "*.po" EXCLUDE
  PATTERN "*.pot" EXCLUDE
)

# MSVC Flags
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
