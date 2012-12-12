include_directories(${CMAKE_SOURCE_DIR}/bin/util ${djv_core_include_dirs})

add_executable(djv_cineon_info djv_cineon_info.cxx)

target_link_libraries(djv_cineon_info ${djv_core_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_cineon_info PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    TARGETS djv_cineon_info
    RUNTIME DESTINATION bin)

add_executable(djv_dpx_info djv_dpx_info.cxx)

target_link_libraries(djv_dpx_info ${djv_core_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_dpx_info PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    TARGETS djv_dpx_info
    RUNTIME DESTINATION bin)

