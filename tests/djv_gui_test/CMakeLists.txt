set(src
    djv_buttons_test.cpp
    djv_choice_widget_test.cpp
    djv_color_widget_test.cpp
    djv_file_browser_test.cpp
    djv_image_io_widget_test.cpp
    djv_multiline_label_test.cpp
    djv_prefs_test.cpp
    djv_push_button_test.cpp
    djv_slider_test.cpp
    djv_sliders_test.cpp
    djv_tabs_test.cpp
    djv_text_widget_test.cpp)

include_directories(
    ${CMAKE_SOURCE_DIR}/tests/djv_gui
    ${djv_gui_include_dirs})

foreach (i ${src})

    get_filename_component(j ${i} NAME_WE)
  
    add_executable(${j} ${i})
  
    target_link_libraries(${j} ${djv_gui_libs})
  
endforeach (i)

