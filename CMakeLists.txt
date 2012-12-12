#-------------------------------------------------------------------------------
# DJV Imaging - CMake Build Configuration
#
# Contents:
#
# * Information
# * Build Options
# * Software Dependencies
#   * FLTK
#   * GLEW
#   * IlmBase
#   * JPEG
#   * libquicktime
#   * OpenEXR
#   * PortAudio
#   * PNG
#   * QuickTime
#   * TIFF
#   * zlib
# * System Configuration
#   * Apple
#   * Linux
#   * Windows
# * Packaging
# * Internals
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)

#-------------------------------------------------------------------------------
# Information
#-------------------------------------------------------------------------------

# Project name.

project(djv)

# Version.

set(djv_version_major "0")
set(djv_version_minor "9")
set(djv_version_patch "0")

set(djv_version_name
    djv-${djv_version_major}.${djv_version_minor}.${djv_version_patch})

#-------------------------------------------------------------------------------
# Build Options
#-------------------------------------------------------------------------------

# Set the output directories for the build.

set(djv_build_dir ${PROJECT_BINARY_DIR}/build)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${djv_build_dir}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${djv_build_dir}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${djv_build_dir}/bin)

# Set the build type.

#set(CMAKE_BUILD_TYPE Release)

# Set the build architecture.

if (CMAKE_SIZEOF_VOID_P EQUAL 8)

    set(djv_build_arch x64)

else (CMAKE_SIZEOF_VOID_P EQUAL 8)

    set(djv_build_arch x86)

endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

# Enable shared libraries.

set(BUILD_SHARED_LIBS true)

# Enable memory-mapped file I/O support.

add_definitions(-DDJV_MMAP)

# Enable testing.

enable_testing()

# Enable assertions.

add_definitions(-DDJV_ASSERT)

# Compiler options.

if (APPLE)

    # Enable 32-bit builds.

    #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-arch i386")
    #set(djv_build_arch x86)
    
    # Disable warnings from XPM files.

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

    # Disable warnings from printf/sprintf.

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security")
    
elseif (UNIX)

    if (CMAKE_COMPILER_IS_GNUCXX)

        # Enable 32-bit builds.
  
        #set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m32")
        #set(djv_build_arch x86)

        # Disable warnings from XPM files.

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")

        # Disable warnings from printf/sprintf.

        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security")

        # Profiling options.
    
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
        #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    
    endif (CMAKE_COMPILER_IS_GNUCXX)

elseif (WIN32)

endif (APPLE)

# Set the list of image I/O plugins to be used for dependencies.

set(djv_image_io_plugins ${djv_image_io_plugins} djv_cineon)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_dpx)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_iff)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_ifl)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_lut)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_pic)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_ppm)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_rla)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_sgi)
set(djv_image_io_plugins ${djv_image_io_plugins} djv_targa)

#-------------------------------------------------------------------------------
# Software Dependencies
#-------------------------------------------------------------------------------

include(FindOpenGL)

if (APPLE)

elseif (UNIX)

    include(FindX11)
  
endif (APPLE)

# Set whether local or system versions of the software dependencies are used.
# Locally built versions of the software dependencies are assumed to be within
# the directory specified by the DJV_DEPS_DIR environment variable. System
# versions are searched for with CMake modules and some hard-coded paths
# (which should be replaced with modules).

set(djv_fltk_local         false)
set(djv_glew_local         false)
set(djv_ilmbase_local      false)
set(djv_jpeg_local         false)
set(djv_libquicktime_local false)
set(djv_openexr_local      false)
set(djv_portaudio_local    false)
set(djv_png_local          false)
set(djv_tiff_local         false)
set(djv_zlib_local         false)

# Set the base directory used for local versions of the software.

set(djv_deps_dir $ENV{DJV_DEPS_DIR})

#-------------------------------------------------------------------------------
# FLTK
#-------------------------------------------------------------------------------

if (djv_fltk_local)

    set(FLTK_FOUND true)
    set(FLTK_INCLUDE_DIR  ${djv_deps_dir}/include)
    set(FLTK_BASE_LIBRARY -L${djv_deps_dir}/lib -lfltk)
    set(FLTK_GL_LIBRARY   -L${djv_deps_dir}/lib -lfltk_gl)

    if (WIN32)

    set(FLTK_BASE_LIBRARY
        ${FLTK_BASE_LIBRARY}
        -lComctl32)

    elseif (APPLE)

    set(FLTK_BASE_LIBRARY
        ${FLTK_BASE_LIBRARY}
        -L${djv_deps_dir}/lib
        -lfltk
        "-framework ApplicationServices"
        "-framework Cocoa"
        -lpthread)

    endif (WIN32)

else (djv_fltk_local)

    include(FindFLTK)

endif (djv_fltk_local)

#-------------------------------------------------------------------------------
# GLEW
#-------------------------------------------------------------------------------

if (djv_glew_local)

    if (WIN32)

        set(GLEW_FOUND       true)
        set(GLEW_INCLUDE_DIR ${djv_deps_dir}/include)
        set(GLEW_LIBRARIES   ${djv_deps_dir}/lib/libglew32.a)

    else (WIN32)
  
        set(GLEW_FOUND       true)
        set(GLEW_INCLUDE_DIR ${djv_deps_dir}/include)
        set(GLEW_LIBRARIES   -L${djv_deps_dir}/lib -lGLEW)

    endif (WIN32)
  
else (djv_glew_local)

    # \todo Write a CMake plugin for finding GLEW.

    set(GLEW_FOUND       true)
    set(GLEW_INCLUDE_DIR /usr/include)
    set(GLEW_LIBRARIES   -L/usr/lib/x86_64-linux-gnu -lGLEW)
  
endif (djv_glew_local)

#-------------------------------------------------------------------------------
# IlmBase
#-------------------------------------------------------------------------------

if (djv_ilmbase_local)

    set(ILMBASE_FOUND       true)
    set(ILMBASE_INCLUDE_DIR ${djv_deps_dir}/include/OpenEXR)
    set(ILMBASE_LIBRARIES
        -L${djv_deps_dir}/lib -lHalf -lIlmThread -lImath -lIex)

else (djv_ilmbase_local)

    # \todo Write a CMake plugin for finding ilmbase.

    set(ILMBASE_FOUND       true)
    set(ILMBASE_INCLUDE_DIR /usr/include/OpenEXR)
    set(ILMBASE_LIBRARIES   -L/usr/lib -lHalf -lIlmThread -lImath -lIex)

endif (djv_ilmbase_local)

#-------------------------------------------------------------------------------
# JPEG
#-------------------------------------------------------------------------------

if (djv_jpeg_local)

    set(JPEG_FOUND       true)
    set(JPEG_INCLUDE_DIR ${djv_deps_dir}/include)
    set(JPEG_LIBRARIES   -L${djv_deps_dir}/lib -ljpeg)

else (djv_jpeg_local)

    include(FindJPEG)

endif (djv_jpeg_local)

if (JPEG_FOUND)

    set(djv_image_io_plugins ${djv_image_io_plugins} djv_jpeg)

endif (JPEG_FOUND)

#-------------------------------------------------------------------------------
# libquicktime
#-------------------------------------------------------------------------------

if (djv_libquicktime_local)

    #set(LIBQUICKTIME_FOUND      true)
    set(LIBQUICKTIME_INCLUDE_DIR ${djv_deps_dir}/include)
    set(LIBQUICKTIME_LIBRARIES   -L${djv_deps_dir}/lib -lquicktime)

else (djv_libquicktime_local)

    # \todo Write a CMake plugin for finding libquicktime.

    #set(LIBQUICKTIME_FOUND       true)
    set(LIBQUICKTIME_INCLUDE_DIR /usr/include)
    set(LIBQUICKTIME_LIBRARIES   -lquicktime)

endif (djv_libquicktime_local)

if (LIBQUICKTIME_FOUND)

    set(djv_image_io_plugins ${djv_image_io_plugins} djv_libquicktime)

endif (LIBQUICKTIME_FOUND)

#-------------------------------------------------------------------------------
# OpenEXR
#-------------------------------------------------------------------------------

if (djv_openexr_local)

    set(OPENEXR_FOUND       true)
    set(OPENEXR_INCLUDE_DIR ${djv_deps_dir}/include/OpenEXR)
    set(OPENEXR_LIBRARIES   -L${djv_deps_dir}/lib -lIlmImf -lz)

else (djv_openexr_local)

    # \todo Write a CMake plugin for finding OpenEXR.

    set(OPENEXR_FOUND       true)
    set(OPENEXR_INCLUDE_DIR /usr/include/OpenEXR)
    set(OPENEXR_LIBRARIES   -L/usr/lib -lIlmImf -lz)

endif (djv_openexr_local)

if (OPENEXR_FOUND)

    set(djv_image_io_plugins ${djv_image_io_plugins} djv_openexr)

endif (OPENEXR_FOUND)

#-------------------------------------------------------------------------------
# PortAudio
#-------------------------------------------------------------------------------

if (djv_portaudio_local)

    set(PORTAUDIO_FOUND       true)
    set(PORTAUDIO_INCLUDE_DIR ${djv_deps_dir}/include)
    set(PORTAUDIO_LIBRARIES   -L${djv_deps_dir}/lib -lportaudio)

    if (WIN32)

    elseif (APPLE)

        set(PORTAUDIO_LIBRARIES
            ${PORTAUDIO_LIBRARIES}
            /System/Library/Frameworks/AudioUnit.framework
            /System/Library/Frameworks/AudioToolBox.framework)

    elseif (UNIX)
  
#        set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES} -ljack -lasound)
        set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES}
            /usr/lib/x86_64-linux-gnu/libjack.so.0)

    endif (WIN32)

else (djv_portaudio_local)

    # \todo Write a CMake plugin for finding PortAudio.

    set(PORTAUDIO_FOUND       true)
    set(PORTAUDIO_INCLUDE_DIR /usr/include)
    set(PORTAUDIO_LIBRARIES   -lportaudio)

    if (WIN32)

    elseif (APPLE)

        set(PORTAUDIO_LIBRARIES
            ${PORTAUDIO_LIBRARIES}
            /System/Library/Frameworks/AudioUnit.framework
            /System/Library/Frameworks/AudioToolBox.framework)

    elseif (UNIX)

        #set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES} -ljack -lasound)
        set(PORTAUDIO_LIBRARIES ${PORTAUDIO_LIBRARIES}
            /usr/lib/x86_64-linux-gnu/libjack.so.0)

    endif (WIN32)

endif (djv_portaudio_local)

#-------------------------------------------------------------------------------
# PNG
#-------------------------------------------------------------------------------

if (djv_png_local)

    set(PNG_FOUND        true)
    set(PNG_INCLUDE_DIRS ${djv_deps_dir}/include)
    set(PNG_LIBRARIES    -L${djv_deps_dir}/lib -lpng -lz)
  
else (djv_png_local)

    include(FindPNG)
  
endif (djv_png_local)

if (PNG_FOUND)

    set(djv_image_io_plugins ${djv_image_io_plugins} djv_png)

endif (PNG_FOUND)

#-------------------------------------------------------------------------------
# QuickTime
#
# \todo Note that QuickTime is only available with 32-bit builds.
#-------------------------------------------------------------------------------

if (WIN32)

    #set(QUICKTIME_FOUND true)
  
    set(
        QUICKTIME_INCLUDE_DIR
        "C:\\Program Files\\QuickTime SDK\\CIncludes")
  
    set(
        QUICKTIME_LIBRARIES
        "C:\\Program Files\\QuickTime SDK\\Libraries\\QTMLClient.lib")
  
elseif (APPLE)

    #set(QUICKTIME_FOUND true)
  
    set(QUICKTIME_LIBRARIES
        /System/Library/Frameworks/QuickTime.framework
        /System/Library/Frameworks/CoreAudio.framework
        /System/Library/Frameworks/Carbon.framework)

endif (WIN32)

if (QUICKTIME_FOUND)

    set(djv_image_io_plugins ${djv_image_io_plugins} djv_quicktime)

endif (QUICKTIME_FOUND)

#-------------------------------------------------------------------------------
# TIFF
#-------------------------------------------------------------------------------

if (djv_tiff_local)

    set(TIFF_FOUND       true)
    set(TIFF_INCLUDE_DIR ${djv_deps_dir}/include)
    set(TIFF_LIBRARIES   -L${djv_deps_dir}/lib -ltiff -ljpeg -lz)

else (djv_tiff_local)

    include(FindTIFF)

endif (djv_tiff_local)

if (TIFF_FOUND)

    set(djv_image_io_plugins ${djv_image_io_plugins} djv_tiff)

endif (TIFF_FOUND)

#-------------------------------------------------------------------------------
# zlib
#-------------------------------------------------------------------------------

if (djv_zlib_local)

    set(ZLIB_FOUND        true)
    set(ZLIB_INCLUDE_DIRS ${djv_deps_dir}/include)
    set(ZLIB_LIBRARIES    -L${djv_deps_dir}/lib -lz)

else (djv_zlib_local)

    include(FindZLIB)

endif (djv_zlib_local)

#-------------------------------------------------------------------------------
# System Configuration
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Apple
#-------------------------------------------------------------------------------

if (APPLE)

    set(djv_system_name OSX)

    add_definitions(-DDJV_OSX)

    configure_file(
        ${PROJECT_SOURCE_DIR}/etc/OSX/startup.sh.in
        ${PROJECT_BINARY_DIR}/startup.sh)

    configure_file(
        ${PROJECT_SOURCE_DIR}/etc/OSX/Info.plist.in
        ${PROJECT_BINARY_DIR}/Info.plist)

endif (APPLE)

#-------------------------------------------------------------------------------
# Linux
#-------------------------------------------------------------------------------

if (APPLE)

elseif (UNIX)

    string(COMPARE EQUAL ${CMAKE_SYSTEM_NAME} Linux _TMP)

    if (_TMP)

        set(djv_system_name Linux)

        add_definitions(-DDJV_LINUX)
        add_definitions(-DDJV_X11)

    endif (_TMP)

endif (APPLE)

#-------------------------------------------------------------------------------
# Windows
#-------------------------------------------------------------------------------

if (WIN32)

    set(djv_system_name Windows)

    add_definitions(-DDJV_WINDOWS)

    # \todo Find a better way to locate the redistributable system libraries.

    add_definitions(-D_BIND_TO_CURRENT_VCLIBS_VERSION=1)

    #message("CMAKE_CL_64 = "${CMAKE_CL_64})

    if (CMAKE_CL_64)

        set(MSVC90_REDIST_DIR
            "C:/Program Files (x86)/Microsoft Visual Studio 9.0/VC/redist")

    else (CMAKE_CL_64)

        set(MSVC90_REDIST_DIR
            "C:/Program Files/Microsoft Visual Studio 9.0/VC/redist")

    endif (CMAKE_CL_64)

    include(InstallRequiredSystemLibraries)

    message("CMAKE_MSVC_ARCH = "${CMAKE_MSVC_ARCH})
    message("CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS = "
        ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
    message("__install__libs = "${__install__libs})

endif (WIN32)

#message("djv_system_name = "${djv_system_name})

#-------------------------------------------------------------------------------
# Packaging
#-------------------------------------------------------------------------------

if (WIN32)

    set(CPACK_GENERATOR NSIS ZIP)

    set(CPACK_NSIS_MUI_ICON ${PROJECT_SOURCE_DIR}/etc/Windows/projector.ico)
    set(CPACK_NSIS_MUI_UNICON ${PROJECT_SOURCE_DIR}/etc/Windows/projector.ico)

    set(djv_bundle_name win)
    
elseif (APPLE)

    set(CPACK_GENERATOR Bundle)
    set(CPACK_PACKAGING_INSTALL_PREFIX "")

    set(CPACK_BUNDLE_NAME ${djv_version_name})
    set(CPACK_BUNDLE_ICON ${PROJECT_SOURCE_DIR}/etc/OSX/djv.icns)
    set(CPACK_BUNDLE_PLIST ${PROJECT_BINARY_DIR}/Info.plist)
    set(CPACK_BUNDLE_STARTUP_COMMAND ${PROJECT_BINARY_DIR}/startup.sh)
    set(CPACK_PACKAGE_ICON ${PROJECT_SOURCE_DIR}/etc/OSX/djv.icns)

    set(djv_bundle_name osx)
    
elseif (UNIX)

    set(CPACK_GENERATOR DEB RPM TGZ)
    set(CPACK_PACKAGING_INSTALL_PREFIX /usr/local/djv)
    
    set(djv_bundle_name linux)
    set(djv_install_rpath ${CPACK_PACKAGING_INSTALL_PREFIX}/lib)

endif (WIN32)

set(CPACK_PACKAGE_VERSION_MAJOR ${djv_version_major})
set(CPACK_PACKAGE_VERSION_MINOR ${djv_version_minor})
set(CPACK_PACKAGE_VERSION_PATCH ${djv_version_patch})
set(CPACK_PACKAGE_CONTACT http://djv.sourceforge.net)
set(CPACK_PACKAGE_DESCRIPTION_FILE ${PROJECT_SOURCE_DIR}/README.txt)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "DJV Imaging provides professional movie playback software for use in "
    "film, VFX, and computer animation.")

set(CPACK_PACKAGE_EXECUTABLES
    djv_view "djv_view - Image and movie viewer")

set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LEGAL.txt)

set(CPACK_PACKAGE_FILE_NAME
    ${djv_version_name}_${djv_bundle_name}-${djv_build_arch})

#message(${CPACK_PACKAGE_FILE_NAME})

#-------------------------------------------------------------------------------
# Internals
#-------------------------------------------------------------------------------

# Set the configuration file.

configure_file(
    ${PROJECT_SOURCE_DIR}/djv_config.h.in
    ${PROJECT_BINARY_DIR}/djv_config.h
    @ONLY)

# Set the libraries used for linking to djv_core.

set(djv_core_libs djv_core ${djv_core_deps})

# Set the djv_core include directories.

set(djv_core_include_dirs
    ${CMAKE_SOURCE_DIR}/lib/djv_core
    ${CMAKE_BINARY_DIR}
    ${FLTK_INCLUDE_DIR}
    ${X11_INCLUDE_DIR}
    ${ILMBASE_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR})

# Set the djv_core dependencies.

set(djv_core_deps
    ${FLTK_GL_LIBRARY}
    ${FLTK_BASE_LIBRARY}
    ${X11_LIBRARIES}
    ${X11_Xft_LIB}
    ${X11_Xpm_LIB}
    ${X11_Xinerama_LIB}
    ${ILMBASE_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS})

# Set the libraries used for linking to djv_core.

set(djv_core_libs djv_core ${djv_core_deps})

# Set the djv_gui include directories.

set(djv_gui_include_dirs
    ${CMAKE_SOURCE_DIR}/lib/djv_gui
    ${CMAKE_SOURCE_DIR}/lib/djv_core
    ${CMAKE_BINARY_DIR}
    ${FLTK_INCLUDE_DIR}
    ${X11_INCLUDE_DIR}
    ${ILMBASE_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR})

# Set the djv_gui dependencies.

set(djv_gui_deps
    djv_core
    ${FLTK_GL_LIBRARY}
    ${FLTK_BASE_LIBRARY}
    ${X11_LIBRARIES}
    ${X11_Xft_LIB}
    ${X11_Xpm_LIB}
    ${X11_Xinerama_LIB}
    ${ILMBASE_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS})

# Set the libraries used for linking to djv_gui.

set(djv_gui_libs djv_gui ${djv_gui_deps})

# Add sub-directories.

add_subdirectory(bin)
add_subdirectory(lib)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(doc)

# \todo CPack needs to come last?

include(CPack)
