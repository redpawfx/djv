set(header
    djv_jpeg.h
    djv_jpeg_load.h
    djv_jpeg_save.h
    djv_jpeg_save_widget.h)

set(source
    djv_jpeg.cpp
    djv_jpeg_load.cpp
    djv_jpeg_save.cpp
    djv_jpeg_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_jpeg
    ${JPEG_INCLUDE_DIR}
    ${djv_gui_include_dirs})

add_library(djv_jpeg MODULE ${header} ${source})

target_link_libraries(djv_jpeg ${JPEG_LIBRARIES} ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_jpeg PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_jpeg
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

