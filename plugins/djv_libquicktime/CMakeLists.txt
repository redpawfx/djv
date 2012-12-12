set(header
    djv_libquicktime.h
    djv_libquicktime_load.h
    djv_libquicktime_load_widget.h
    djv_libquicktime_save.h
    djv_libquicktime_save_widget.h)

set(source
    djv_libquicktime.cpp
    djv_libquicktime_load.cpp
    djv_libquicktime_load_widget.cpp
    djv_libquicktime_save.cpp
    djv_libquicktime_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_libquicktime
    ${LIBQUICKTIME_INCLUDE_DIR}
    ${djv_gui_include_dirs})

add_library(djv_libquicktime MODULE ${header} ${source})

target_link_libraries(
    djv_libquicktime
    ${LIBQUICKTIME_LIBRARIES}
    ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_libquicktime PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_libquicktime
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

