file(GLOB src *.cxx)
#set(src test_str.cxx)

set(
  test
  test_box
  test_directory
  test_directory_fnc
  test_file
  test_file_fnc
  test_file_seq
  test_math_range
  test_matrix
  test_string
  test_vector
)
#set(test)

include_directories(${djv_base_inc})

foreach (i ${src})

  get_filename_component(j ${i} NAME_WE)
  
  add_executable(${j} ${i})
  
  add_dependencies(${j} ${djv_base_dep})
  
  target_link_libraries(${j} ${djv_base_lib})
  
endforeach (i)

foreach (i ${test})
  add_test(${i} ${djv_build_dir}/bin/${i})
endforeach (i)

