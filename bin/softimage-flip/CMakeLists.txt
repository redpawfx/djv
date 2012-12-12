add_executable(flip flip.cxx)

if (djv_install_rpath)
  set_target_properties(
    flip PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS flip
  RUNTIME DESTINATION bin
)

if (APPLE)
  DJV_OSX_INSTALL_BIN(flip)
endif (APPLE)

