set(
  header
  application.h
  base.h
  cache.h
  file_group.h
  file_prefs.h
  file_save.h
  help_group.h
  image_base.h
  image_display_profile.h
  image_group.h
  image_prefs.h
  input_base.h
  input_prefs.h
  playback_base.h
  playback_group.h
  playback_prefs.h
  shortcut.h
  tool_color_picker.h
  tool_group.h
  tool_histogram.h
  tool_info.h
  tool_magnify.h
  view_base.h
  view_group.h
  view_prefs.h
  view_widget.h
  widget.h
  window.h
  window_base.h
  window_prefs.h
)

set(
  src
  application.cxx
  cache.cxx
  cache_etc.cxx
  command_line.cxx
  file_etc.cxx
  file_group.cxx
  file_prefs.cxx
  file_prefs_widget.cxx
  file_save.cxx
  help_etc.cxx
  help_group.cxx
  image_display_profile.cxx
  image_etc.cxx
  image_group.cxx
  image_prefs.cxx
  image_prefs_widget.cxx
  input_etc.cxx
  input_prefs.cxx
  input_prefs_widget.cxx
  playback_etc.cxx
  playback_group.cxx
  playback_prefs.cxx
  playback_prefs_widget.cxx
  shortcut_etc.cxx
  shortcut_prefs.cxx
  shortcut_prefs_widget.cxx
  tool_color_picker.cxx
  tool_etc.cxx
  tool_group.cxx
  tool_histogram_dialog.cxx
  tool_histogram_widget.cxx
  tool_info.cxx
  tool_magnify.cxx
  view_etc.cxx
  view_group.cxx
  view_prefs.cxx
  view_prefs_widget.cxx
  view_widget.cxx
  widget.cxx
  window.cxx
  window_etc.cxx
  window_factory.cxx
  window_prefs.cxx
  window_prefs_widget.cxx
)

if (WIN32)

  set(header ${header} ../../lib/djv_ui/winrc.h)
  set(src ${src} ../../lib/djv_ui/win.rc)

endif (WIN32)

include_directories(${djv_ui_inc})

add_executable(
  djv_view
  WIN32
  ${header} ${src} win_main.cxx
)

add_dependencies(
  djv_view
  ${djv_ui_dep}
)

target_link_libraries(
  djv_view
  ${djv_ui_lib}
)

set_target_properties(
  djv_view PROPERTIES
  COMPILE_FLAGS "${djv_compiler_flags}"
  LINK_FLAGS "${djv_link_flags}"
)

if (djv_install_rpath)
  set_target_properties(
    djv_view PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  TARGETS djv_view
  RUNTIME DESTINATION bin
)

if (APPLE)
  DJV_OSX_INSTALL_BIN(djv_view)
endif (APPLE)

# XXX Windows command line version (.com).

if (WIN32)

  add_executable(djv_view. ${header} ${src} main.cxx)
  
  add_dependencies(djv_view. ${djv_ui_dep})
  
  target_link_libraries(djv_view. ${djv_ui_lib})
  
  set_target_properties(
    djv_view.
    PROPERTIES
    SUFFIX com
  )
  
  if (djv_install_rpath)
    set_target_properties(
      djv_view.
      PROPERTIES
      INSTALL_RPATH "${djv_install_rpath}"
      COMPILER_FLAGS ${djv_compiler_flags}
      LINK_FLAGS ${djv_link_flags}
    )
  endif (djv_install_rpath)
  
  install(
    TARGETS djv_view.
    RUNTIME DESTINATION bin
  )

endif (WIN32)

