function(parakeet_linker_hack ret linker_lib_list)
  set(local_linker_lib_list ${linker_lib_list})

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
