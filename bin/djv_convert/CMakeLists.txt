set(header
    djv_convert.h)

set(source
    djv_convert.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/bin/djv_convert
    ${djv_core_include_dirs})

add_executable(djv_convert ${header} ${source})

target_link_libraries(djv_convert ${djv_core_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_convert PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

if (djv_image_io_plugins)

    add_dependencies(djv_convert ${djv_image_io_plugins})

endif (djv_image_io_plugins)

install(
    TARGETS djv_convert
    RUNTIME DESTINATION bin)

