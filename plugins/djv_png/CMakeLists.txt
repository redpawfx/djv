set(header
    djv_png.h
    djv_png_load.h
    djv_png_save.h)

set(source
    djv_png.cpp
    djv_png_load.cpp
    djv_png_save.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_png
    ${PNG_INCLUDE_DIR}
    ${djv_gui_include_dirs})

add_library(djv_png MODULE ${header} ${source})

target_link_libraries(djv_png ${PNG_LIBRARIES} ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_png PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_png
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

