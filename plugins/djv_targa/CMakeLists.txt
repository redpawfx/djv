set(header
    djv_targa.h
    djv_targa_load.h
    djv_targa_save.h
    djv_targa_save_widget.h)

set(source
    djv_targa.cpp
    djv_targa_load.cpp
    djv_targa_save.cpp
    djv_targa_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_targa
    ${djv_gui_include_dirs})

add_library(djv_targa MODULE ${header} ${source})

target_link_libraries(djv_targa ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_targa PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_targa
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

