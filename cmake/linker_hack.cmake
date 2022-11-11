function(parakeet_linker_hack ret linker_lib_list)
  set(local_linker_lib_list ${linker_lib_list})
  if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("HACK: Replace wxWidgets with debug-static path")
    list(TRANSFORM local_linker_lib_list REPLACE
      "static/lib/(wx[a-zA-Z]+[0-9]*u?)_([a-zA-Z0-9]+).lib$"
      "static/debug/lib/\\1d_\\2.lib"
    )
    list(TRANSFORM local_linker_lib_list REPLACE
      "static/lib/(wx[a-zA-Z]+[0-9]*u?).lib$"
      "static/debug/lib/\\1d.lib"
    )
  endif()

  if (NOT MSVC)
    list(TRANSFORM local_linker_lib_list REPLACE "^.+/lib(png|jpeg|tiff)\\.a$" "-l\\1")
    message("HACK: Don't static link lib(png|jpeg|tiff) in Linux,")
    message("      As it conflicts with libGTK3 and crashes at times.")

    string(REGEX MATCH "/libwx[^;]+\.a;" OUT1 "${local_linker_lib_list}")
    if (NOT "${OUT1}" STREQUAL "")
      list(FILTER local_linker_lib_list EXCLUDE REGEX "^-lwx")
      message("      * Also removing any dynamically linked wxWidgets...")
    endif()
    message("      local_linker_lib_list: ${local_linker_lib_list}")
  endif()

  SET(${ret} "${local_linker_lib_list}" PARENT_SCOPE)
endfunction()
