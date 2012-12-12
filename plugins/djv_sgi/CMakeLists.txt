set(header
    djv_sgi.h
    djv_sgi_load.h
    djv_sgi_save.h
    djv_sgi_save_widget.h)

set(source
    djv_sgi.cpp
    djv_sgi_load.cpp
    djv_sgi_save.cpp
    djv_sgi_save_widget.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_sgi
    ${djv_gui_include_dirs})

add_library(djv_sgi MODULE ${header} ${source})

target_link_libraries(djv_sgi ${djv_gui_libs})

if (djv_install_rpath)

  set_target_properties(
        djv_sgi PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_sgi
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

