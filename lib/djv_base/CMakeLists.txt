set(
  header
  application_def.h
  application.h
  base.h
  box2_inline.h
  box3_inline.h
  box_def.h
  box_fnc.h
  box_fnc_inline.h
  box.h
  box_inline.h
  debug_def.h
  debug.h
  debug_inline.h
  directory_def.h
  directory_fnc.h
  directory.h
  error_def.h
  error.h
  export.h
  file_def.h
  file_fnc.h
  file.h
  file_inline.h
  file_io_def.h
  file_io.h
  file_io_inline.h
  file_seq_def.h
  file_seq_fnc.h
  file_seq_fnc_inline.h
  file_seq.h
  file_seq_inline.h
  list_def.h
  list_fnc.h
  list_fnc_inline.h
  list.h
  list_inline.h
  math_def.h
  math.h
  math_inline.h
  matrix3_inline.h
  matrix4_inline.h
  matrix_def.h
  matrix_fnc.h
  matrix_fnc_inline.h
  matrix.h
  matrix_inline.h
  memory_def.h
  memory.h
  memory_inline.h
  plugin_def.h
  plugin.h
  set_def.h
  set.h
  set_inline.h
  string_def.h
  string_fnc.h
  string_fnc_inline.h
  string.h
  string_inline.h
  system.h
  time_def.h
  time.h
  type.h
  user.h
  vector2_inline.h
  vector3_inline.h
  vector_def.h
  vector_fnc.h
  vector_fnc_inline.h
  vector.h
  vector_inline.h
)

set(
  src
  application.cxx
  application_etc.cxx
  debug.cxx
  directory.cxx
  directory_fnc.cxx
  error.cxx
  file.cxx
  file_etc.cxx
  file_filter.cxx
  file_fnc.cxx
  file_io.cxx
  file_io_etc.cxx
  file_path.cxx
  file_seq.cxx
  file_sort.cxx
  file_split.cxx
  math.cxx
  memory.cxx
  plugin.cxx
  string.cxx
  string_fnc.cxx
  string_format.cxx
  system.cxx
  time.cxx
  user.cxx
  vector_fnc.cxx
)

include_directories(${djv_base_inc})

add_library(
  djv_base
  ${header}
  ${src}
)

target_link_libraries(
  djv_base
  ${_djv_base_lib}
)

set_target_properties(
  djv_base PROPERTIES
  COMPILE_FLAGS "${djv_compiler_flags}"
  LINK_FLAGS "${djv_link_flags}"
)
if (djv_install_rpath)
  set_target_properties(
    djv_base PROPERTIES
    INSTALL_RPATH "${djv_install_rpath}"
  )
endif (djv_install_rpath)

install(
  FILES ${header}
  DESTINATION include/djv_base
)

install(
  TARGETS djv_base
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

if (APPLE)
  DJV_OSX_INSTALL_LIB(djv_base)
endif (APPLE)

