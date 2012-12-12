set(
  header
  base.h
  load.h
  save.h
)
set(
  src
  base.cxx
  etc.cxx
  load.cxx
  save.cxx
)

set(
  ui_header
  base.h
  load.h
  save.h
  ui_base.h
  ui_load.h
  ui_save.h
)
set(
  ui_src
  etc.cxx
  ui_base.cxx
  ui_load.cxx
  ui_save.cxx
)

include_directories(${djv_ui_inc} ${OPENEXR_INCLUDE_DIR})

add_library(djv_openexr MODULE ${header} ${src})
add_library(djv_openexr_ui MODULE ${ui_header} ${ui_src})

add_dependencies(djv_openexr ${djv_image_dep})
add_dependencies(djv_openexr_ui ${djv_ui_dep})

target_link_libraries(djv_openexr ${djv_image_lib} ${OPENEXR_LIBRARIES})
target_link_libraries(djv_openexr_ui ${djv_ui_lib} ${OPENEXR_LIBRARIES})

if (djv_install_rpath)
  set_target_properties(
    djv_openexr PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
  set_target_properties(
    djv_openexr_ui PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_openexr
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)
install(
  TARGETS djv_openexr_ui
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)

if (APPLE)
  DJV_OSX_INSTALL_PLUGIN(djv_openexr)
  DJV_OSX_INSTALL_PLUGIN(djv_openexr_ui)
endif (APPLE)

