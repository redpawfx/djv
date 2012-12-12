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
  djv_info
  ${header}
  ${src}
)

add_dependencies(
  djv_info
  ${djv_image_dep}
)

target_link_libraries(
  djv_info
  ${djv_image_lib}
)

set_target_properties(
  djv_info PROPERTIES
  COMPILE_FLAGS "${djv_compiler_flags}"
  LINK_FLAGS "${djv_link_flags}"
)
if (djv_install_rpath)
  set_target_properties(
    djv_info PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_info
  RUNTIME DESTINATION bin
)

if (APPLE)
  DJV_OSX_INSTALL_BIN(djv_info)
endif (APPLE)

