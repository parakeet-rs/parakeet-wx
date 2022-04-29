project(um-desktop)

include_directories($<TARGET_PROPERTY:um-crypto,INTERFACE_INCLUDE_DIRECTORIES>)

if(MSVC)
  # Using DPI aware manifest from wxWidgets
  set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")  
endif()

find_package(wxWidgets REQUIRED)
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

file(GLOB SOURCES *.cpp **/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_definitions(${PROJECT_NAME} PRIVATE
  ${wxWidgets_DEFINITIONS}
  "$<$<CONFIG:DEBUG>:${wxWidgets_DEFINITIONS_DEBUG}>"
)
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE um-crypto_static)

if (MSVC)
  # FIXME: How to get arm working? Don't have a Windows arm device.
  add_definitions(-D_WIN32=1)
  if("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    add_definitions(-D_WIN64=1)
  endif()
  target_sources(${PROJECT_NAME} PRIVATE app.rc)
endif()
