set(header
    djv_rla.h
    djv_rla_load.h)

set(source
    djv_rla.cpp
    djv_rla_load.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_rla
    ${djv_gui_include_dirs})

add_library(djv_rla MODULE ${header} ${source})

target_link_libraries(djv_rla ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_rla PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_rla
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

