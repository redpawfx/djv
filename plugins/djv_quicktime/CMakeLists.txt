set(header
    djv_quicktime.h
    djv_quicktime_load.h
    djv_quicktime_load_widget.h
    djv_quicktime_save.h
    djv_quicktime_save_widget.h)

set(source
    djv_quicktime.cpp
    djv_quicktime_load.cpp
    djv_quicktime_load_widget.cpp
    djv_quicktime_save.cpp
    djv_quicktime_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_quicktime
    ${QUICKTIME_INCLUDE_DIR}
    ${djv_gui_include_dirs})

add_library(djv_quicktime MODULE ${header} ${source})

target_link_libraries(djv_quicktime ${QUICKTIME_LIBRARIES} ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_quicktime PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_quicktime
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
