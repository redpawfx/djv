set(
    header
    djv_assert.h
    djv_box2_inline.h
    djv_box3_inline.h
    djv_box.h
    djv_box_inline.h
    djv_color.h
    djv_color_inline.h
    djv_color_profile.h
    djv_core_application.h
    djv_core_export.h
    djv_debug.h
    djv_debug_inline.h
    djv_directory.h
    djv_error.h
    djv_file.h
    djv_file_inline.h
    djv_file_io.h
    djv_file_io_inline.h
    djv_gl.h
    djv_gl_context.h
    djv_gl_image.h
    djv_gl_inline.h
    djv_gl_offscreen_buffer.h
    djv_image.h
    djv_image_tag.h
    djv_image_io.h
    djv_list.h
    djv_list_inline.h
    djv_math.h
    djv_math_inline.h
    djv_matrix3_inline.h
    djv_matrix4_inline.h
    djv_matrix.h
    djv_matrix_inline.h
    djv_memory.h
    djv_memory_inline.h
    djv_memory_buffer.h
    djv_memory_buffer_inline.h
    djv_pixel.h
    djv_pixel_data.h
    djv_pixel_data_inline.h
    djv_pixel_inline.h
    djv_plugin.h
    djv_range.h
    djv_range_inline.h
    djv_seq.h
    djv_seq_inline.h
    djv_set.h
    djv_set_inline.h
    djv_speed.h
    djv_string.h
    djv_string_inline.h
    djv_system.h
    djv_time.h
    djv_timer.h
    djv_type.h
    djv_user.h
    djv_vector2_inline.h
    djv_vector3_inline.h
    djv_vector.h
    djv_vector_inline.h)

set(
    source
    djv_assert.cpp
    djv_color.cpp
    djv_color_profile.cpp
    djv_core_application.cpp
    djv_debug.cpp
    djv_directory.cpp
    djv_error.cpp
    djv_file.cpp
    djv_file_filter.cpp
    djv_file_util.cpp
    djv_file_io.cpp
    djv_file_path.cpp
    djv_file_sort.cpp
    djv_file_split.cpp
    djv_gl.cpp
    djv_gl_context.cpp
    djv_gl_image.cpp
    djv_gl_image_draw.cpp
    djv_gl_offscreen_buffer.cpp
    djv_image.cpp
    djv_image_tag.cpp
    djv_image_io.cpp
    djv_math.cpp
    djv_memory.cpp
    djv_pixel_convert.cpp
    djv_pixel.cpp
    djv_pixel_data.cpp
    djv_plugin.cpp
    djv_seq.cpp
    djv_speed.cpp
    djv_string.cpp
    djv_string_format.cpp
    djv_system.cpp
    djv_time.cpp
    djv_timer.cpp
    djv_user.cpp
    djv_vector.cpp)

if (APPLE)

    set(header
        ${header}
        djv_cgl_context_private.h)

    set(source
        ${source}
        djv_cgl_context_private.cpp)

elseif (UNIX)

    set(header
        ${header}
        djv_glx_context_private.h)

    set(source
        ${source}
        djv_glx_context_private.cpp)

elseif (WIN32)

    set(header
        ${header}
        djv_wgl_context_private.h)

    set(source
        ${source}
        djv_wgl_context_private.cpp)

endif (APPLE)

include_directories(${djv_core_include_dirs})

add_library(djv_core ${header} ${source})

target_link_libraries(djv_core ${djv_core_deps})

if (djv_install_rpath)

    set_target_properties(
        djv_core PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    FILES ${header}
    DESTINATION include)

install(
    TARGETS djv_core
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

