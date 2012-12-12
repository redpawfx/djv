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
  save.h
  ui_save.h
)
set(
  ui_src
  etc.cxx
  ui_save.cxx
)

include_directories(${djv_ui_inc} ${JPEG_INCLUDE_DIR})

add_library(djv_jpeg MODULE ${header} ${src})
add_library(djv_jpeg_ui MODULE ${ui_header} ${ui_src})

add_dependencies(djv_jpeg ${djv_image_dep})
add_dependencies(djv_jpeg_ui ${djv_ui_dep})

target_link_libraries(djv_jpeg ${djv_image_lib} ${JPEG_LIBRARIES})
target_link_libraries(djv_jpeg_ui ${djv_ui_lib} ${JPEG_LIBRARIES})

if (djv_install_rpath)
  set_target_properties(
    djv_jpeg PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
  set_target_properties(
    djv_jpeg_ui PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_jpeg
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)
install(
  TARGETS djv_jpeg_ui
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)

if (APPLE)
  DJV_OSX_INSTALL_PLUGIN(djv_jpeg)
  DJV_OSX_INSTALL_PLUGIN(djv_jpeg_ui)
endif (APPLE)

