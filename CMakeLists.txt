#-------------------------------------------------------------------------------
# DJV Imaging - CMake Build Configuration
#
# Contents:
#
# * Software Information
# * Software Components
#   * Libraries
#   * Plugins
#   * Programs
#   * Tests
# * Third-Party Software
# * Build Options
# * Packaging Options
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 2.6)

#-------------------------------------------------------------------------------
# Software Information
#-------------------------------------------------------------------------------

# Project name.

project(djv)

# Version.

set(djv_version_major "0")
set(djv_version_minor "8")
set(djv_version_patch "3")
set(djv_version_beta "-pre2")

#-------------------------------------------------------------------------------
# Software Components
#-------------------------------------------------------------------------------

# Build a version with minimal functionality for debugging. This disables most
# image I/O plugins and third-party software.

#set(djv_build_minimal true)

# Libraries
# ---------

# List of libraries to build:

set(
  djv_build_lib
  djv_base
  djv_image
  djv_ui
)

#set(djv_build_lib)

# Plugins
# -------

# QuickTime compatible?

if (WIN32)

  if (NOT CMAKE_CL_64)    
    set(djv_quicktime true)
  endif (NOT CMAKE_CL_64)

elseif (APPLE)

  set(djv_quicktime true)

elseif (UNIX)

  set(djv_quicktime true)

endif (WIN32)

# List of plugins to build:

if (NOT djv_build_minimal)

  if (WIN32)

    set(
      djv_build_plugin
      djv_openexr
      djv_jpeg
      djv_png
      djv_tiff
      djv_vlut
    )
  
    if (djv_quicktime)

      set(
        djv_build_plugin
        ${djv_build_plugin}
        djv_quicktime
      )

    endif (djv_quicktime)

  elseif (APPLE)

    set(
      djv_build_plugin
      djv_openexr
      djv_jpeg
      djv_png
      djv_tiff
    )
  
    if (djv_quicktime)

      set(
        djv_build_plugin
        ${djv_build_plugin}
        djv_quicktime
      )

    endif (djv_quicktime)

  elseif (UNIX)

    set(
      djv_build_plugin
      djv_openexr
      djv_jpeg
      djv_png
      djv_tiff
      djv_libquicktime
#      djv_vlut
    )

  endif (WIN32)

endif (NOT djv_build_minimal)

#set(djv_build_plugin)

# Programs
# --------

# List of programs to build:

if (NOT djv_build_minimal)

  set(
    djv_build_bin
    djv_ls
    djv_info
    djv_convert
    djv_view
    util
    softimage-flip
  )

else (NOT djv_build_minimal)

  set(
    djv_build_bin
    djv_ls
    djv_info
    djv_convert
    djv_view
  )

endif (NOT djv_build_minimal)

#set(djv_build_bin)

# Tests
# -----

# List of tests to build:

if (NOT djv_build_minimal)

  set(
    djv_build_test
    djv_base
    djv_image
    djv_ui
    glsl
    image_play
    image_play2
    image_view
  )
  
  if (djv_quicktime)
  
    set(
      djv_build_test
      ${djv_build_test}
      audio
    )
    
  endif (djv_quicktime)
  
endif (NOT djv_build_minimal)

#set(djv_build_test)
#set(djv_build_test djv_ui)
#set(djv_build_test image_play2)

#-------------------------------------------------------------------------------
# Third-Party Software
#-------------------------------------------------------------------------------

# Uncomment the following line to use system versions of third-party software:

set(djv_third_party_use_system true)


# XXX This allows you to disable dependency-checking for building third-party
# software. During devleopment you can disable this to speed-up the build
# system.

set(djv_build_third_party true)

#-------------------------------------------------------------------------------
# Build Options
#-------------------------------------------------------------------------------

# Temporary directory to use for build.

set(djv_build_dir ${PROJECT_BINARY_DIR}/build)

# Enable shared libraries.

set(BUILD_SHARED_LIBS true)

# Enable memory-mapped file I/O support.

add_definitions(-DDJV_MMAP)

# Enable testing.

enable_testing()

# Profiling options.

#set(djv_compiler_flags -pg)
#set(djv_link_flags -pg)

# Debugging options.

add_definitions(-DDJV_ASSERT)
#add_definitions(-DDJV_DEBUG)

#-------------------------------------------------------------------------------
# Packaging Options
#-------------------------------------------------------------------------------

if (WIN32)

  set(CPACK_GENERATOR NSIS ZIP)

elseif (APPLE)

  set(CPACK_GENERATOR Bundle)
  set(CPACK_PACKAGING_INSTALL_PREFIX "")

elseif (UNIX)

  set(CPACK_GENERATOR DEB RPM TGZ)
  set(CPACK_PACKAGING_INSTALL_PREFIX /usr/local/djv)

endif (WIN32)

#-------------------------------------------------------------------------------

include(cmake_internal.txt)

