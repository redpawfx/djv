set(header
    djv_info.h)

set(source
    djv_info.cpp)

include_directories(${CMAKE_SOURCE_DIR}/bin/djv_info ${djv_core_include_dirs})

add_executable(djv_info ${header} ${source})

target_link_libraries(djv_info ${djv_core_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_info PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

if (djv_image_io_plugins)

    add_dependencies(djv_info ${djv_image_io_plugins})

endif (djv_image_io_plugins)

install(
    TARGETS djv_info
    RUNTIME DESTINATION bin)

