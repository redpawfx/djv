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

include_directories(${djv_image_inc} ${VLUT_INCLUDE_DIR})

add_library(djv_vlut MODULE ${header} ${src})

add_dependencies(djv_vlut ${djv_image_dep})

target_link_libraries(djv_vlut ${djv_image_lib} ${VLUT_LIBRARIES})

if (djv_install_rpath)
  set_target_properties(
    djv_vlut PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_vlut
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${djv_lib_subdir}
)

if (APPLE)
  DJV_OSX_INSTALL_PLUGIN(djv_vlut)
endif (APPLE)

