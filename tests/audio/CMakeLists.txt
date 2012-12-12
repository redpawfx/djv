file(GLOB header *.h)
file(GLOB src *.cxx)

include_directories(
  ${djv_ui_inc}
  ${PORTAUDIO_INCLUDE_DIR}
  ${QUICKTIME_INCLUDE_DIR}
)

add_executable(test_audio ${header} ${src})

add_dependencies(test_audio ${djv_ui_dep})

target_link_libraries(
  test_audio
  ${djv_ui_lib}
  ${QUICKTIME_LIBRARIES}
  ${PORTAUDIO_LIBRARIES}
)

target_link_libraries(test_audio ${djv_ui_lib})

if (djv_install_rpath)
  set_target_properties(
    test_audio PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(TARGETS test_audio RUNTIME DESTINATION bin)

if (APPLE)
  DJV_OSX_INSTALL_BIN(test_audio)
endif (APPLE)

