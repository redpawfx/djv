set(header
    djv_ifl.h
    djv_ifl_load.h)

set(source
    djv_ifl_load.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_ifl
    ${djv_gui_include_dirs})

add_library(djv_ifl MODULE ${header} ${source})

target_link_libraries(djv_ifl ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_ifl PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_ifl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

