set(header
    djv_glsl_application.h
    djv_glsl_blur.h
    djv_glsl_color.h
    djv_glsl_edge.h
    djv_glsl_exposure.h
    djv_glsl_op.h
    djv_glsl_levels.h
    djv_glsl_scale.h
    djv_glsl_sharpen.h
    djv_glsl_util.h
    djv_glsl_widget.h)

set(source
    djv_glsl_application.cpp
    djv_glsl_blur.cpp
    djv_glsl_color.cpp
    djv_glsl_op.cpp
    djv_glsl_edge.cpp
    djv_glsl_exposure.cpp
    djv_glsl_levels.cpp
    djv_glsl_scale.cpp
    djv_glsl_sharpen.cpp
    djv_glsl_util.cpp
    djv_glsl_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/tests/djv_glsl_test
    ${djv_gui_include_dirs})

add_executable(djv_glsl_test ${header} ${source})

target_link_libraries(djv_glsl_test ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_glsl_test PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

if (djv_image_io_plugins)

    add_dependencies(djv_glsl_test ${djv_image_io_plugins})

endif (djv_image_io_plugins)

