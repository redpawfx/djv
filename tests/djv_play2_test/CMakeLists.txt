set(header
    djv_play2_application.h
    djv_play2_image.h
    djv_play2_widget.h)

set(source
    djv_play2_application.cpp
    djv_play2_image.cpp
    djv_play2_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/tests/djv_play2_test
    ${djv_gui_include_dirs})

add_executable(djv_play2_test ${header} ${source})

target_link_libraries(djv_play2_test ${djv_gui_libs})

if (djv_install_rpath)

  set_target_properties(
        djv_play2_test PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

if (djv_image_io_plugins)

    add_dependencies(djv_play2_test ${djv_image_io_plugins})

endif (djv_image_io_plugins)

