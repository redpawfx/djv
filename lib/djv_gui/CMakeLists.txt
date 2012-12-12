set(
    header
    djv_abstract_slider.h
    djv_abstract_text_widget.h
    djv_abstract_value_widget.h
    djv_abstract_value_widget_inline.h
    djv_abstract_widget.h
    djv_abstract_widget_inline.h
    djv_application.h  
    djv_application_about_dialog.h
    djv_application_info_dialog.h
    djv_application_message_dialog.h
    djv_callback.h
    djv_callback_inline.h
    djv_check_button.h
    djv_check_button_group.h
    djv_choice_dialog.h
    djv_choice_edit_widget.h
    djv_choice_tool_button.h
    djv_choice_widget.h
    djv_color_dialog.h
    djv_color_swatch.h
    djv_color_widget.h
    djv_columns.h
    djv_cursor.h
    djv_dialog.h
    djv_file_browser.h
    djv_file_browser_prefs.h
    djv_file_browser_private.h
    djv_file_widget.h
    djv_float_edit.h
    djv_float_edit_slider.h
    djv_float_display.h
    djv_float_slider.h
    djv_font.h
    djv_form_widget.h
    djv_frame.h
    djv_frame_group.h
    djv_gl_widget.h
    djv_group_box.h
    djv_gui_export.h
    djv_icon.h
    djv_image_io_widget.h
    djv_image_label.h
    djv_image_prefs.h
    djv_image_view.h
    djv_inc_button.h
    djv_input_dialog.h
    djv_int_edit.h
    djv_int_edit_slider.h
    djv_int_display.h
    djv_int_slider.h
    djv_label.h
    djv_layout.h
    djv_layout_inline.h
    djv_layout_item.h
    djv_layout_item_inline.h
    djv_message_dialog.h
    djv_menu.h
    djv_multi_choice_dialog.h
    djv_multiline_label.h
    djv_multiline_text_display.h
    djv_pixel_widget.h
    djv_prefs_dialog.h
    djv_prefs.h
    djv_prefs_inline.h
    djv_progress_dialog.h
    djv_push_button.h
    djv_question_dialog.h
    djv_radio_button.h
    djv_radio_button_group.h
    djv_row_layout.h
    djv_scroll.h
    djv_shortcut.h
    djv_shortcut_widget.h
    djv_shuttle.h
    djv_stack_layout.h
    djv_style.h
    djv_tabs.h
    djv_text_edit.h
    djv_text_display.h
    djv_tool_button.h
    djv_tooltip_prefs.h
    djv_vector_widget.h
    djv_widget.h
    djv_widget_inline.h
    djv_window.h)

set(
    source
    djv_abstract_slider.cpp
    djv_abstract_text_widget.cpp
    djv_abstract_widget.cpp
    djv_abstract_value_widget.cpp
    djv_application.cpp
    djv_application_about_dialog.cpp
    djv_application_info_dialog.cpp
    djv_application_message_dialog.cpp
    djv_callback.cpp
    djv_check_button.cpp
    djv_check_button_group.cpp
    djv_choice_dialog.cpp
    djv_choice_edit_widget.cpp
    djv_choice_tool_button.cpp
    djv_choice_widget.cpp
    djv_color_dialog.cpp
    djv_color_swatch.cpp
    djv_color_widget.cpp
    djv_columns.cpp
    djv_cursor.cpp
    djv_dialog.cpp
    djv_file_browser.cpp
    djv_file_browser_prefs.cpp
    djv_file_browser_private.cpp
    djv_file_widget.cpp
    djv_float_edit.cpp
    djv_float_edit_slider.cpp
    djv_float_display.cpp
    djv_float_slider.cpp
    djv_font.cpp
    djv_form_widget.cpp
    djv_frame.cpp
    djv_frame_group.cpp
    djv_gl_widget.cpp
    djv_group_box.cpp
    djv_icon.cpp
    djv_image_io_widget.cpp
    djv_image_label.cpp
    djv_image_prefs.cpp
    djv_image_view.cpp
    djv_inc_button.cpp
    djv_input_dialog.cpp
    djv_int_edit.cpp
    djv_int_edit_slider.cpp
    djv_int_display.cpp
    djv_int_slider.cpp
    djv_label.cpp
    djv_layout.cpp
    djv_layout_item.cpp
    djv_menu.cpp
    djv_menu_popup.cpp
    djv_message_dialog.cpp
    djv_multi_choice_dialog.cpp
    djv_multiline_label.cpp
    djv_multiline_text_display.cpp
    djv_pixel_widget.cpp
    djv_prefs.cpp
    djv_prefs_dialog.cpp
    djv_progress_dialog.cpp
    djv_push_button.cpp
    djv_question_dialog.cpp
    djv_radio_button.cpp
    djv_radio_button_group.cpp
    djv_row_layout.cpp
    djv_scroll.cpp
    djv_shortcut.cpp
    djv_shortcut_widget.cpp
    djv_shuttle.cpp
    djv_stack_layout.cpp
    djv_style.cpp
    djv_style_prefs.cpp
    djv_tabs.cpp
    djv_text_edit.cpp
    djv_text_display.cpp
    djv_tool_button.cpp
    djv_tooltip_prefs.cpp
    djv_vector_widget.cpp
    djv_widget.cpp
    djv_window.cpp)

if (WIN32)

    set(header ${header} winrc.h)

endif (WIN32)

include_directories(${djv_gui_include_dirs})

add_library(djv_gui ${header} ${source})

target_link_libraries(djv_gui ${djv_gui_deps})

if (djv_install_rpath)

    set_target_properties(
        djv_gui PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_gui
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
