include_directories(${djv_ui_inc})

add_executable(test_image_view test_image_view.cxx)

add_dependencies(test_image_view ${djv_ui_dep})

target_link_libraries(test_image_view ${djv_ui_lib})

