set(header
    djv_lut.h
    djv_lut_load.h
    djv_lut_load_widget.h
    djv_lut_save.h
    djv_lut_save_widget.h)

set(source
    djv_lut.cpp
    djv_lut_load.cpp
    djv_lut_load_widget.cpp
    djv_lut_save.cpp
    djv_lut_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_lut
    ${djv_gui_include_dirs})

add_library(djv_lut MODULE ${header} ${source})

target_link_libraries(djv_lut ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_lut PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_lut
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

