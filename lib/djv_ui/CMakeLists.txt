set(
  header
  application_def.h
  application_dialog_def.h
  application_dialog.h
  application.h
  base_def.h
  base.h
  callback_def.h
  callback.h
  callback_inline.h
  check_button_def.h
  check_button.h
  choice_def.h
  choice.h
  color_def.h
  color.h
  columns_def.h
  columns.h
  dialog_def.h
  dialog.h
  export.h
  file_browser_def.h
  file_browser.h
  file_browser_prefs.h
  file_def.h
  file.h
  float_value_def.h
  float_value.h
  frame_def.h
  frame.h
  gl_def.h
  gl.h
  global_def.h
  global.h
  group_box_def.h
  group_box.h
  icon_def.h
  icon.h
  image.h
  image_io_cineon.h
  image_io_def.h
  image_io.h
  image_io_ppm.h
  image_pixel_def.h
  image_pixel.h
  image_prefs_def.h
  image_prefs.h
  image_view_def.h
  image_view.h
  int_value_def.h
  int_value.h
  label_def.h
  label.h
  layout_def.h
  layout.h
  layout_inline.h
  layout_item_def.h
  layout_item.h
  layout_item_inline.h
  layout_row_def.h
  layout_row.h
  layout_stack_def.h
  layout_stack.h
  menu_def.h
  menu.h
  prefs_def.h
  prefs_dialog_def.h
  prefs_dialog.h
  prefs.h
  prefs_inline.h
  push_button_def.h
  push_button.h
  radio_button_def.h
  radio_button.h
  scroll_def.h
  scroll.h
  shortcut_def.h
  shortcut.h
  shuttle_def.h
  shuttle.h
  slider_def.h
  slider.h
  style_def.h
  style.h
  tabs_def.h
  tabs.h
  text_def.h
  text.h
  tool_button_def.h
  tool_button.h
  value_def.h
  value.h
  value_inline.h
  vector_def.h
  vector.h
  widget_def.h
  widget.h
  widget_inline.h
  window_def.h
  window.h
)

if (NOT djv_build_minimal)

  set(
    header
    ${header}
    image_io_cineon.h
    image_io_def.h
    image_io_dpx.h
    image_io_iff.h
    image_io_lut.h
    image_io_sgi.h
    image_io_targa.h
  )

endif (NOT djv_build_minimal)

set(
  src
  application.cxx
  application_dialog.cxx
  base.cxx
  base_etc.cxx
  callback.cxx
  check_button.cxx
  check_button_group.cxx
  choice.cxx
  choice_edit.cxx
  color_etc.cxx
  color_swatch.cxx
  color_widget.cxx
  columns.cxx
  dialog.cxx
  dialog_choice.cxx
  dialog_color.cxx
  dialog_etc.cxx
  dialog_input.cxx
  dialog_message.cxx
  dialog_multi_choice.cxx
  dialog_progress.cxx
  dialog_question.cxx
  file.cxx
  file_browser_callback.cxx
  file_browser_dialog.cxx
  file_browser_etc.cxx
  file_browser_item.cxx
  file_browser_menu.cxx
  file_browser_prefs.cxx
  file_browser_prefs_widget.cxx
  file_browser_widget.cxx
  float_value.cxx
  frame.cxx
  gl.cxx
  global.cxx
  group_box.cxx
  icon.cxx
  image.cxx
  image_io.cxx
  image_io_etc.cxx
  image_io_ppm.cxx
  image_io_prefs.cxx
  image_pixel.cxx
  image_prefs.cxx
  image_view.cxx
  int_value.cxx
  label.cxx
  label_image.cxx
  label_multiline.cxx
  layout.cxx
  layout_etc.cxx
  layout_item.cxx
  layout_row.cxx
  layout_stack.cxx
  menu.cxx
  menu_etc.cxx
  menu_popup.cxx
  prefs.cxx
  prefs_dialog.cxx
  prefs_dialog_etc.cxx
  prefs_dialog_widget.cxx
  push_button.cxx
  radio_button.cxx
  radio_button_group.cxx
  scroll.cxx
  shortcut.cxx
  shortcut_etc.cxx
  shuttle.cxx
  slider.cxx
  style.cxx
  style_etc.cxx
  style_prefs.cxx
  tabs.cxx
  text.cxx
  text_display.cxx
  text_edit.cxx
  tool_button.cxx
  tool_button_choice.cxx
  value.cxx
  vector.cxx
  widget.cxx
  widget_base.cxx
  window.cxx
)

if (NOT djv_build_minimal)

  set(
    src
    ${src}
    image_io_cineon.cxx
    image_io_dpx.cxx
    image_io_iff.cxx
    image_io_lut.cxx
    image_io_sgi.cxx
    image_io_targa.cxx
  )

endif (NOT djv_build_minimal)

#if (WIN32)
#  set(header ${header} winrc.h)
#  set(src ${src} win.rc)
#endif (WIN32)

include_directories(${djv_ui_inc})

add_library(
  djv_ui
  ${header}
  ${src}
)

add_dependencies(
  djv_ui
  ${djv_base_dep}
)

target_link_libraries(
  djv_ui
  ${_djv_ui_lib}
)

set_target_properties(
  djv_ui PROPERTIES
  COMPILE_FLAGS "${djv_compiler_flags}"
  LINK_FLAGS "${djv_link_flags}"
)
if (djv_install_rpath)
  set_target_properties(
    djv_ui PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  FILES ${header}
  DESTINATION include/djv_ui
)

install(
  TARGETS djv_ui
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

if (APPLE)
  DJV_OSX_INSTALL_LIB(djv_ui)
endif (APPLE)

