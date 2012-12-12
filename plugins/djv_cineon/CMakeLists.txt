set(header
    djv_cineon.h
    djv_cineon_header.h
    djv_cineon_load.h
    djv_cineon_load_widget.h
    djv_cineon_save.h
    djv_cineon_save_widget.h)

set(source
    djv_cineon.cpp
    djv_cineon_header.cpp
    djv_cineon_load.cpp
    djv_cineon_load_widget.cpp
    djv_cineon_save.cpp
    djv_cineon_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_cineon
    ${djv_gui_include_dirs})

add_library(djv_cineon MODULE ${header} ${source})

target_link_libraries(djv_cineon ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_cineon PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_cineon
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

