set(header
    djv_ppm.h
    djv_ppm_load.h
    djv_ppm_save.h
    djv_ppm_save_widget.h)

set(source
    djv_ppm.cpp
    djv_ppm_load.cpp
    djv_ppm_save.cpp
    djv_ppm_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_ppm
    ${djv_gui_include_dirs})

add_library(djv_ppm MODULE ${header} ${source})

target_link_libraries(djv_ppm ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_ppm PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_ppm
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

