project(um-desktop)

include_directories()

if(MSVC)
  # Using DPI aware manifest from wxWidgets
  set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")  
endif()

set(Boost_NO_WARN_NEW_VERSIONS 1)
find_package(OpenSSL REQUIRED) # used by um-crypto
find_package(Boost COMPONENTS filesystem nowide thread REQUIRED system)
find_package(wxWidgets COMPONENTS base aui core adv propgrid REQUIRED)
find_package(ZLIB REQUIRED) # Linux build complains without zlib.

# Optional: use mediainfo for audio format detection.
find_package(MediaInfoLib CONFIG)
if (MediaInfoLib_FOUND)
  find_package(ZenLib CONFIG)
  set(MEDIAINFO_LIBRARIES ${ZenLib_LIBRARY} ${MediaInfoLib_LIBRARY})
  add_definitions(-DBUILD_WITH_MEDIAINFO=1)
else()
  add_definitions(-DBUILD_WITH_MEDIAINFO=0)
  set(MEDIAINFO_LIBRARIES)
endif()

if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("HACK: Replace wxWidgets with debug-static path")
  list(TRANSFORM wxWidgets_LIBRARIES REPLACE
    "static/lib/(wx[a-zA-Z]+[0-9]*u?)_([a-zA-Z0-9]+).lib$"
    "static/debug/lib/\\1d_\\2.lib"
  )
  list(TRANSFORM wxWidgets_LIBRARIES REPLACE
    "static/lib/(wx[a-zA-Z]+[0-9]*u?).lib$"
    "static/debug/lib/\\1d.lib"
  )
  message("wxWidgets_LIBRARIES: ${wxWidgets_LIBRARIES}")
endif()

include_directories(
  ${wxWidgets_INCLUDE_DIRS}
  ${BOOST_INCLUDE_DIRS}
  $<TARGET_PROPERTY:um-crypto,INTERFACE_INCLUDE_DIRECTORIES>
)

link_libraries(
  ${MEDIAINFO_LIBRARIES}
  ${wxWidgets_LIBRARIES}
  ${Boost_LIBRARIES}
  ${ZLIB_LIBRARIES}
  um-crypto_static
)

file(GLOB_RECURSE SOURCES *.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${wxWidgets_DEFINITIONS}
  "$<$<CONFIG:DEBUG>:${wxWidgets_DEFINITIONS_DEBUG}>"
)
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

if (MSVC)
  # FIXME: How to get arm working? Don't have a Windows arm device.
  add_definitions(-D_WIN32=1)
  if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    add_definitions(-D_WIN64=1)
  endif()
  target_sources(${PROJECT_NAME} PRIVATE app.rc)
endif()
