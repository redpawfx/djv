file(GLOB src *.cxx)

set(
  test
  test_color
  test_pixel
)

include_directories(${djv_image_inc})

foreach (i ${src})

  get_filename_component(j ${i} NAME_WE)
  
  add_executable(${j} ${i})
  
  add_dependencies(${j} ${djv_image_dep})
  
  target_link_libraries(${j} ${djv_image_lib})
  
endforeach (i)

foreach (i ${test})
  add_test(${i} ${djv_build_dir}/bin/${i})
endforeach (i)

