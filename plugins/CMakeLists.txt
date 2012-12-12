add_subdirectory(djv_cineon)
add_subdirectory(djv_dpx)
add_subdirectory(djv_iff)
add_subdirectory(djv_ifl)

if (JPEG_FOUND)

    add_subdirectory(djv_jpeg)

endif (JPEG_FOUND)

if (LIBQUICKTIME_FOUND)

    add_subdirectory(djv_libquicktime)

endif (LIBQUICKTIME_FOUND)

add_subdirectory(djv_lut)

if (OPENEXR_FOUND)

    add_subdirectory(djv_openexr)

endif (OPENEXR_FOUND)

add_subdirectory(djv_pic)

if (PNG_FOUND)

    add_subdirectory(djv_png)

endif (PNG_FOUND)

add_subdirectory(djv_ppm)

if (QUICKTIME_FOUND)

    add_subdirectory(djv_quicktime)

endif (QUICKTIME_FOUND)

add_subdirectory(djv_rla)
add_subdirectory(djv_sgi)
add_subdirectory(djv_targa)

if (TIFF_FOUND)

    add_subdirectory(djv_tiff)

endif (TIFF_FOUND)
