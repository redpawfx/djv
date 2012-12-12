set(
  header
  application.h
  image.h
  widget.h
)

set(
  src
  application.cxx
  image.cxx
  widget.cxx
)

include_directories(${djv_ui_inc})

add_executable(test_image_play2 ${header} ${src})

add_dependencies(test_image_play2 ${djv_ui_dep})

target_link_libraries(test_image_play2 ${djv_ui_lib})

