add_executable(softimage-flip flip.cxx)

if (djv_install_rpath)

    set_target_properties(
        softimage-flip PROPERTIES
        INSTALL_RPATH "${djv_install_rpath}")

endif (djv_install_rpath)

install(
    TARGETS softimage-flip
    RUNTIME DESTINATION bin)

