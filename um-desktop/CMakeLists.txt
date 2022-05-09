project(um-desktop)

add_compile_options(-DAPP_INTERNAL_NAME=\"Parakeet\")

if(MSVC)
  # Using DPI aware manifest from wxWidgets
  set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")  
endif()

set(Boost_NO_WARN_NEW_VERSIONS 1)
find_package(Boost COMPONENTS thread REQUIRED system filesystem)
find_package(wxWidgets COMPONENTS base aui core adv propgrid REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(Botan REQUIRED) # From um-crypto
find_package(ZLIB REQUIRED) # Linux build complains without zlib.

set(QMD_LINK_TARGETS
  ${Boost_LIBRARIES}
  ${BOTAN_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${wxWidgets_LIBRARIES}
  rapidjson
  um-crypto_static
)

if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("HACK: Replace wxWidgets with debug-static path")
  list(TRANSFORM QMD_LINK_TARGETS REPLACE
    "static/lib/(wx[a-zA-Z]+[0-9]*u?)_([a-zA-Z0-9]+).lib$"
    "static/debug/lib/\\1d_\\2.lib"
  )
  list(TRANSFORM QMD_LINK_TARGETS REPLACE
    "static/lib/(wx[a-zA-Z]+[0-9]*u?).lib$"
    "static/debug/lib/\\1d.lib"
  )
endif()

if (NOT MSVC)
  list(TRANSFORM QMD_LINK_TARGETS REPLACE "^.+/lib(png|jpeg|tiff)\\.a$" "-l\\1")
  message("HACK: Don't static link lib(png|jpeg|tiff) in Linux,")
  message("      As it conflicts with libGTK3 and crashes at times.")

  string(REGEX MATCH "/libwx[^;]+\.a;" OUT1 "${QMD_LINK_TARGETS}")
  if (NOT "${OUT1}" STREQUAL "")
    list(FILTER QMD_LINK_TARGETS EXCLUDE REGEX "^-lwx")
    message("      * Also removing any dynamically linked wxWidgets...")
  endif()
  message("      QMD_LINK_TARGETS: ${QMD_LINK_TARGETS}")
endif()

include_directories(
  ${wxWidgets_INCLUDE_DIRS}
  ${BOOST_INCLUDE_DIRS}
  $<TARGET_PROPERTY:um-crypto,INTERFACE_INCLUDE_DIRECTORIES>
)

link_libraries(${QMD_LINK_TARGETS})

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
