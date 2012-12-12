file(GLOB src *.cxx)
#set(src test_button.cxx)
#set(src test_buttons.cxx)
#set(src test_sliders.cxx)
#set(src test_color_widget.cxx)
#set(src test_prefs.cxx)

include_directories(${djv_ui_inc})

foreach (i ${src})

  get_filename_component(j ${i} NAME_WE)
  
  add_executable(${j} ${i})
  
  add_dependencies(${j} ${djv_ui_dep})
  
  target_link_libraries(${j} ${djv_ui_lib})
  
endforeach (i)

foreach (i ${test})
  add_test(${i} ${djv_build_dir}/bin/${i})
endforeach (i)

