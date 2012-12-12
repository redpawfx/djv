set(
  header
  application_def.h
  application.h
  base.h
  color_def.h
  color.h
  color_inline.h
  color_profile_def.h
  color_profile.h
  data_def.h
  data.h
  data_inline.h
  export.h
  gl_data_def.h
  gl_data.h
  gl_def.h
  gl.h
  gl_inline.h
  gl_offscreen_def.h
  gl_offscreen.h
  image_def.h
  image.h
  image_util_def.h
  image_util.h
  io.h
  io_ppm.h
  pixel.h
  pixel_inline.h
  tag_def.h
  tag.h
  time_def.h
  time.h
)

if (NOT djv_build_minimal)

  set(
    header
    ${header}
    io_cineon.h
    io_def.h
    io_dpx.h
    io.h
    io_iff.h
    io_ifl.h
    io_lut.h
    io_pic.h
    io_rla.h
    io_sgi.h
    io_targa.h
  )

endif (NOT djv_build_minimal)

set(
  src
  application.cxx
  application_command_line.cxx
  application_etc.cxx
  color.cxx
  color_etc.cxx
  color_profile.cxx
  data.cxx
  data_etc.cxx
  data_gradient.cxx
  gl-1.2_data.cxx
  gl-2.0_data_draw.cxx
  gl-2.0_data_etc.cxx
  gl-2.0_data_scale.cxx
  gl.cxx
  gl_data.cxx
  gl_data_average.cxx
  gl_data_copy.cxx
  gl_data_etc.cxx
  gl_data_histogram.cxx
  gl_offscreen.cxx
  gl_offscreen_etc.cxx
  gl_offscreen_fbo.cxx
  image.cxx
  image_util.cxx
  io.cxx
  io_etc.cxx
  io_ppm_data.cxx
  io_ppm_etc.cxx
  io_ppm_load.cxx
  io_ppm_save.cxx
  pixel_convert.cxx
  pixel_etc.cxx
  tag.cxx
  time.cxx
)

if (NOT djv_build_minimal)

  set(
    src
    ${src}
    io_cineon_color.cxx
    io_cineon_data.cxx
    io_cineon_etc.cxx
    io_cineon_info.cxx
    io_cineon_load.cxx
    io_cineon_save.cxx
    io_dpx_etc.cxx
    io_dpx_info.cxx
    io_dpx_load.cxx
    io_dpx_save.cxx
    io_iff_etc.cxx
    io_iff_info.cxx
    io_iff_load.cxx
    io_iff_save.cxx
    io_iff_data.cxx
    io_ifl_load.cxx
    io_lut_etc.cxx
    io_lut_load.cxx
    io_lut_save.cxx
    io_pic_data.cxx
    io_pic_etc.cxx
    io_pic_load.cxx
    io_rla_data.cxx
    io_rla_load.cxx
    io_sgi_data.cxx
    io_sgi_etc.cxx
    io_sgi_info.cxx
    io_sgi_load.cxx
    io_sgi_save.cxx
    io_targa_data.cxx
    io_targa_etc.cxx
    io_targa_info.cxx
    io_targa_load.cxx
    io_targa_save.cxx
  )

endif (NOT djv_build_minimal)

if (WIN32)

  set(
    src
    ${src}
    wgl.cxx
    wgl_offscreen.cxx
    wgl_offscreen_pbuffer.cxx
  )  

elseif (APPLE)

  set(
    src
    ${src}
    cgl.cxx
    cgl_offscreen.cxx
    cgl_offscreen_pbuffer.cxx
  )  

elseif (UNIX)

  set(
    src
    ${src}
    glx.cxx
    glx_offscreen.cxx
    glx_offscreen_pbuffer.cxx
  )  

endif (WIN32)

include_directories(${djv_image_inc})

add_library(
  djv_image
  ${header}
  ${src}
)

add_dependencies(
  djv_image
  ${djv_base_dep}
)

target_link_libraries(
  djv_image
  ${_djv_image_lib}
)

set_target_properties(
  djv_image PROPERTIES
  COMPILE_FLAGS "${djv_compiler_flags}"
  LINK_FLAGS "${djv_link_flags}"
)
if (djv_install_rpath)
  set_target_properties(
    djv_image PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  FILES ${header}
  DESTINATION include/djv_image
)

install(
  TARGETS djv_image
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

if (APPLE)
  DJV_OSX_INSTALL_LIB(djv_image)
endif (APPLE)

