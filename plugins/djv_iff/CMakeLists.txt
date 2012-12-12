set(header
    djv_iff.h
    djv_iff_load.h
    djv_iff_save.h
    djv_iff_save_widget.h)

set(source
    djv_iff.cpp
    djv_iff_load.cpp
    djv_iff_save.cpp
    djv_iff_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_iff
    ${djv_gui_include_dirs})

add_library(djv_iff MODULE ${header} ${source})

target_link_libraries(djv_iff ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_iff PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_iff
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

