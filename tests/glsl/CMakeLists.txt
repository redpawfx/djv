file(GLOB header *.h)
file(GLOB src *.cxx)

include_directories(${djv_ui_inc})

add_executable(test_glsl ${header} ${src})

add_dependencies(test_glsl ${djv_ui_dep})

target_link_libraries(test_glsl ${djv_ui_lib})

if (djv_install_rpath)
  set_target_properties(
    test_glsl PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(TARGETS test_glsl RUNTIME DESTINATION bin)

if (APPLE)
  DJV_OSX_INSTALL_BIN(test_glsl)
endif (APPLE)

