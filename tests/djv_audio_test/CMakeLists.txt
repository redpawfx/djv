set(header
    djv_audio_test.h)

set(source
    djv_audio_test.cpp
    djv_audio_window.cpp)

if (QUICKTIME_FOUND)

    set(source
        ${source}
        djv_audio_quicktime.cpp)

elseif (LIBQUICKTIME_FOUND)

    set(source
        ${source}
        djv_audio_libquicktime.cpp)

endif (QUICKTIME_FOUND)

include_directories(
    ${CMAKE_SOURCE_DIR}/tests/djv_audio_test
    ${PORTAUDIO_INCLUDE_DIR}
    ${djv_gui_include_dirs})

add_executable(djv_audio_test ${header} ${source})

if (QUICKTIME_FOUND)

    target_link_libraries(
        djv_audio_test
        ${QUICKTIME_LIBRARIES}
        ${PORTAUDIO_LIBRARIES}
        ${djv_gui_libs})

elseif (LIBQUICKTIME_FOUND)

    target_link_libraries(
        djv_audio_test
        ${LIBQUICKTIME_LIBRARIES}
        ${PORTAUDIO_LIBRARIES}
        ${djv_gui_libs})

endif (QUICKTIME_FOUND)

if (djv_install_rpath)

    set_target_properties(
        djv_audio_test PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

if (djv_image_io_plugins)

    add_dependencies(djv_audio_test ${djv_image_io_plugins})

endif (djv_image_io_plugins)

install(
    TARGETS djv_audio_test
    RUNTIME DESTINATION bin)
