set(src
    djv_box_test.cpp
    djv_cmdln_test.cpp
    djv_color_test.cpp
    djv_directory_test.cpp
    djv_file_test.cpp
    djv_io_line_test.cpp
    djv_io_word_test.cpp
    djv_matrix_test.cpp
    djv_pixel_test.cpp
    djv_range_test.cpp
    djv_seq_test.cpp
    djv_string_test.cpp
    djv_timecode_test.cpp
    djv_vector_test.cpp)

set(test
    djv_box_test
    djv_directory_test
    djv_file_test
    djv_matrix_test
    djv_range_test
    djv_seq_test
    djv_string_test
    djv_vector_test)

include_directories(
    ${CMAKE_SOURCE_DIR}/tests/djv_core
    ${djv_core_include_dirs})

foreach (i ${src})

    get_filename_component(j ${i} NAME_WE)
  
    add_executable(${j} ${i})
  
    target_link_libraries(${j} ${djv_core_libs})

endforeach (i)

foreach (i ${test})

    add_test(
        ${i}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${i}${CMAKE_EXECUTABLE_SUFFIX})

endforeach (i)
