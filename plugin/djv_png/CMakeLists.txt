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

include_directories(${djv_image_inc} ${PNG_INCLUDE_DIR})

add_library(djv_png MODULE ${header} ${src})

add_dependencies(djv_png ${djv_image_dep})

target_link_libraries(djv_png ${djv_image_lib} ${PNG_LIBRARIES})

if (djv_install_rpath)
  set_target_properties(
    djv_png PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_png
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)

if (APPLE)
  DJV_OSX_INSTALL_PLUGIN(djv_png)
endif (APPLE)

