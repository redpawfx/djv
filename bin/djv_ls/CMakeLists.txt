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
  djv_ls
  ${header}
  ${src}
)

add_dependencies(
  djv_ls
  ${djv_image_dep}
)

target_link_libraries(
  djv_ls
  ${djv_image_lib}
)

set_target_properties(
  djv_ls PROPERTIES
  COMPILE_FLAGS "${djv_compiler_flags}"
  LINK_FLAGS "${djv_link_flags}"
)
if (djv_install_rpath)
  set_target_properties(
    djv_ls PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)
install(
  TARGETS djv_ls
  RUNTIME DESTINATION bin
)

if (APPLE)
  DJV_OSX_INSTALL_BIN(djv_ls)
endif (APPLE)

