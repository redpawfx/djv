set(
  header
  base.h
  load.h
  save.h
)
set(
  src
  etc.cxx
  data.cxx
  load.cxx
  save.cxx
)

set(
  ui_header
  base.h
  ui_save.h
)
set(
  ui_src
  etc.cxx
  ui_save.cxx
)

include_directories(${djv_ui_inc} ${TIFF_INCLUDE_DIR})

add_library(djv_tiff MODULE ${header} ${src})
add_library(djv_tiff_ui MODULE ${ui_header} ${ui_src})

add_dependencies(djv_tiff ${djv_image_dep})
add_dependencies(djv_tiff_ui ${djv_ui_dep})

target_link_libraries(djv_tiff ${djv_image_lib} ${TIFF_LIBRARIES})
target_link_libraries(djv_tiff_ui ${djv_ui_lib} ${TIFF_LIBRARIES})

if (djv_install_rpath)
  set_target_properties(
    djv_tiff PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
  set_target_properties(
    djv_tiff_ui PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_tiff
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)
install(
  TARGETS djv_tiff_ui
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)

if (APPLE)
  DJV_OSX_INSTALL_PLUGIN(djv_tiff)
  DJV_OSX_INSTALL_PLUGIN(djv_tiff_ui)
endif (APPLE)

