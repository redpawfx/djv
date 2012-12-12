set(header
    djv_tiff.h
    djv_tiff_load.h
    djv_tiff_save.h
    djv_tiff_save_widget.h)

set(source
    djv_tiff.cpp
    djv_tiff_load.cpp
    djv_tiff_save.cpp
    djv_tiff_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_tiff
    ${TIFF_INCLUDE_DIR}
    ${djv_gui_include_dirs})

add_library(djv_tiff MODULE ${header} ${source})

target_link_libraries(djv_tiff ${TIFF_LIBRARIES} ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_tiff PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_tiff
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

