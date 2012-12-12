set(header)

set(source
    djv_play_test.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/tests/djv_play_test
    ${djv_gui_include_dirs})

add_executable(djv_play_test ${header} ${source})

target_link_libraries(djv_play_test ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_play_test PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

if (djv_image_io_plugins)

    add_dependencies(djv_play_test ${djv_image_io_plugins})

endif (djv_image_io_plugins)

