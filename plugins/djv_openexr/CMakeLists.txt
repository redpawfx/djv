set(header
    djv_openexr.h
    djv_openexr_base.h
    djv_openexr_base_widget.h
    djv_openexr_load.h
    djv_openexr_load_widget.h
    djv_openexr_save.h
    djv_openexr_save_widget.h)

set(source
    djv_openexr.cpp
    djv_openexr_base.cpp
    djv_openexr_base_widget.cpp
    djv_openexr_load.cpp
    djv_openexr_load_widget.cpp
    djv_openexr_save.cpp
    djv_openexr_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_openexr
    ${OPENEXR_INCLUDE_DIR}
    ${djv_gui_include_dirs})

add_library(djv_openexr MODULE ${header} ${source})

target_link_libraries(djv_openexr ${OPENEXR_LIBRARIES} ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_openexr PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_openexr
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

