set(header
    djv_pic.h
    djv_pic_load.h)

set(source
    djv_pic.cpp
    djv_pic_load.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/plugins/djv_pic
    ${djv_gui_include_dirs})

add_library(djv_pic MODULE ${header} ${source})

target_link_libraries(djv_pic ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_pic PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_pic
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

