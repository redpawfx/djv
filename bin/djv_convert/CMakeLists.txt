set(
  header
  application.h
  etc.h
)

set(
  src
  application.cxx
  command_line.cxx
  etc.cxx
)

include_directories(${djv_image_inc})

add_executable(
  djv_convert
  ${header}
  ${src}
)

add_dependencies(
  djv_convert
  ${djv_image_dep}
)

target_link_libraries(
  djv_convert
  ${djv_image_lib}
)

if (djv_install_rpath)
  set_target_properties(
    djv_convert PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_convert
  RUNTIME DESTINATION bin
)

if (APPLE)
  DJV_OSX_INSTALL_BIN(djv_convert)
endif (APPLE)

