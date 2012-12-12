include_directories(${djv_image_inc})

set(
  util
  djv_cineon_info
  djv_dpx_info
)

foreach (i ${util})

  file(GLOB src ${i}*.h ${i}*.cxx)
  
  add_executable(${i} ${src})

  add_dependencies(${i} ${djv_image_dep})

  target_link_libraries(${i} ${djv_image_lib})

  if (djv_install_rpath)
    set_target_properties(
      ${i} PROPERTIES
      INSTALL_RPATH "${djv_install_rpath}"
    )
  endif (djv_install_rpath)

  install(
    TARGETS ${i}
    RUNTIME DESTINATION bin
  )
  
  if (APPLE)
    DJV_OSX_INSTALL_BIN(${i})
  endif (APPLE)

endforeach (i)

