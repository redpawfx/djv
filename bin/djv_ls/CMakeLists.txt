set(header
    djv_ls.h)

set(source
    djv_ls.cpp)

include_directories(${CMAKE_SOURCE_DIR}/bin/djv_ls ${djv_core_include_dirs})

add_executable(djv_ls ${header} ${source})

target_link_libraries(djv_ls ${djv_core_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_ls PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

if (djv_image_io_plugins)

    add_dependencies(djv_ls ${djv_image_io_plugins})

endif (djv_image_io_plugins)

install(
    TARGETS djv_ls
    RUNTIME DESTINATION bin)

