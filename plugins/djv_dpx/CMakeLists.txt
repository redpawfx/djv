set(header
    djv_dpx.h
    djv_dpx_header.h
    djv_dpx_load.h
    djv_dpx_load_widget.h
    djv_dpx_save.h
    djv_dpx_save_widget.h)

set(source
    djv_dpx.cpp
    djv_dpx_header.cpp
    djv_dpx_load.cpp
    djv_dpx_load_widget.cpp
    djv_dpx_save.cpp
    djv_dpx_save_widget.cpp
    ${CMAKE_SOURCE_DIR}/plugins/djv_cineon/djv_cineon.cpp
    ${CMAKE_SOURCE_DIR}/plugins/djv_cineon/djv_cineon_header.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_dpx
    ${CMAKE_SOURCE_DIR}/plugins/djv_cineon
    ${djv_gui_include_dirs})

add_library(djv_dpx MODULE ${header} ${source})

target_link_libraries(djv_dpx ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_dpx PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_dpx
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

