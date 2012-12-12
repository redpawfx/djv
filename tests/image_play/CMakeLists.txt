include_directories(${djv_ui_inc})

add_executable(test_image_play test_image_play.cxx)

add_dependencies(test_image_play ${djv_ui_dep})

target_link_libraries(test_image_play ${djv_ui_lib})

