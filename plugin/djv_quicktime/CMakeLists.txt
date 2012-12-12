set(
  header
  base.h
  load.h
  save.h
)
set(
  src
  etc.cxx
  load.cxx
  save.cxx
)

set(
  ui_header
  base.h
  ui_load.h
  ui_save.h
)
set(
  ui_src
  etc.cxx
  ui_load.cxx
  ui_save.cxx
)

include_directories(${djv_ui_inc} ${QUICKTIME_INCLUDE_DIR})

add_library(djv_quicktime MODULE ${header} ${src})
add_library(djv_quicktime_ui MODULE ${ui_header} ${ui_src})

add_dependencies(djv_quicktime ${djv_image_dep})
add_dependencies(djv_quicktime_ui ${djv_ui_dep})

target_link_libraries(djv_quicktime ${djv_image_lib} ${QUICKTIME_LIBRARIES})
target_link_libraries(djv_quicktime_ui ${djv_ui_lib} ${QUICKTIME_LIBRARIES})

if (djv_install_rpath)
  set_target_properties(
    djv_quicktime PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
  set_target_properties(
    djv_quicktime_ui PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_quicktime
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)
install(
  TARGETS djv_quicktime_ui
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)

if (APPLE)
  DJV_OSX_INSTALL_PLUGIN(djv_quicktime)
  DJV_OSX_INSTALL_PLUGIN(djv_quicktime_ui)
endif (APPLE)

