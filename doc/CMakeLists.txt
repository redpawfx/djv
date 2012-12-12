set(
  readme
  ../LEGAL.txt
  ../LICENSE_cmake.txt
  ../LICENSE_ffmpeg.txt
  ../LICENSE_fltk.txt
  ../LICENSE_glew.txt
  ../LICENSE_libjpeg.txt
  ../LICENSE_libpng.txt
  ../LICENSE_libquicktime.txt
  ../LICENSE_libtiff.txt
  ../LICENSE_mesa.txt
  ../LICENSE_openexr.txt
  ../LICENSE_portaudio.txt
  ../LICENSE_qt.txt
  ../LICENSE_zlib.txt
  ../README.txt
  ../CHANGELIST.txt
)

set(src ${readme})

file(GLOB tmp *.html)
set(src ${src} ${tmp})

file(GLOB tmp *.css)
set(src ${src} ${tmp})

file(GLOB tmp *.png)
set(src ${src} ${tmp})

file(GLOB tmp *.gif)
set(src ${src} ${tmp})

install(
  FILES ${src}
  DESTINATION doc
)

if (APPLE)
  DJV_OSX_INSTALL_LINK(doc)
  DJV_OSX_INSTALL_LINK(doc/LEGAL.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_cmake.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_ffmpeg.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_fltk.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_glew.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_libjpeg.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_libpng.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_libquicktime.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_libtiff.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_mesa.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_openexr.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_portaudio.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_qt.txt)
  DJV_OSX_INSTALL_LINK(doc/LICENSE_zlib.txt)
  DJV_OSX_INSTALL_LINK(doc/README.txt)
endif (APPLE)

