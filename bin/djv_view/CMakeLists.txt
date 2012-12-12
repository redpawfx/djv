set(header
    djv_view_application.h
    djv_view_cache.h
    djv_view_color_picker.h
    djv_view_display_profile.h
    djv_view_display_profile_dialog.h
    djv_view_file_group.h
    djv_view_file_prefs.h
    djv_view_file_save.h
    djv_view_help_group.h
    djv_view_histogram_dialog.h
    djv_view_hud_info.h
    djv_view_image.h
    djv_view_image_group.h
    djv_view_image_prefs.h
    djv_view_info_dialog.h
    djv_view_input.h
    djv_view_input_prefs.h
    djv_view_magnify_dialog.h
    djv_view_playback.h
    djv_view_playback_group.h
    djv_view_playback_prefs.h
    djv_view_shortcut.h
    djv_view_shortcut_prefs.h
    djv_view_tool_group.h
    djv_view_view.h
    djv_view_view_group.h
    djv_view_view_prefs.h
    djv_view_view_widget.h
    djv_view_widget.h
    djv_view_window.h
    djv_view_window_prefs.h)

set(source
    djv_view_application.cpp
    djv_view_cache.cpp
    djv_view_color_picker.cpp
    djv_view_display_profile.cpp
    djv_view_display_profile_dialog.cpp
    djv_view_file_group.cpp
    djv_view_file_prefs.cpp
    djv_view_file_save.cpp
    djv_view_help_group.cpp
    djv_view_histogram_dialog.cpp
    djv_view_image.cpp
    djv_view_image_group.cpp
    djv_view_image_prefs.cpp
    djv_view_info_dialog.cpp
    djv_view_input.cpp
    djv_view_input_prefs.cpp
    djv_view_magnify_dialog.cpp
    djv_view_playback.cpp
    djv_view_playback_group.cpp
    djv_view_playback_prefs.cpp
    djv_view_shortcut.cpp
    djv_view_shortcut_prefs.cpp
    djv_view_tool_group.cpp
    djv_view_view.cpp
    djv_view_view_group.cpp
    djv_view_view_prefs.cpp
    djv_view_view_widget.cpp
    djv_view_widget.cpp
    djv_view_window.cpp
    djv_view_window_prefs.cpp)

if (WIN32)

    set(header
        ${header}
        ../../lib/djv_gui/winrc.h)

    set(source
        ${source}
        ../../lib/djv_gui/win.rc)

endif (WIN32)

include_directories(${CMAKE_SOURCE_DIR}/bin/djv_view ${djv_gui_include_dirs})

add_executable(djv_view ${header} ${source} djv_view_win_main.cpp)

target_link_libraries(djv_view ${djv_gui_libs})

if (djv_install_rpath)

    set_target_properties(
        djv_view PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

if (djv_image_io_plugins)

    add_dependencies(djv_view ${djv_image_io_plugins})

endif (djv_image_io_plugins)

install(
    TARGETS djv_view
    RUNTIME DESTINATION bin)

# \todo Windows command line version (.com).

if (WIN32)

    add_executable(djv_view. ${header} ${source} djv_view_main.cpp)
  
    target_link_libraries(djv_view. ${djv_gui_libs})
  
    set_target_properties(
        djv_view.
        PROPERTIES
        SUFFIX com)

    if (djv_install_rpath)
  
        set_target_properties(
            djv_view.
            PROPERTIES
            INSTALL_RPATH "${djv_install_rpath}")
  
    endif (djv_install_rpath)
  
    if (djv_image_io_plugins)
  
        add_dependencies(djv_view. ${djv_image_io_plugins})
  
    endif (djv_image_io_plugins)
  
    install(
        TARGETS djv_view.
        RUNTIME DESTINATION bin)

endif (WIN32)

