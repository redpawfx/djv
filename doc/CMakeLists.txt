set(doc
    ../LEGAL.txt
    ../LICENSE_FLTK.txt
    ../LICENSE_GLEW.txt
    ../LICENSE_IlmBase.txt
    ../LICENSE_JPEG.txt
    ../LICENSE_OpenEXR.txt
    ../LICENSE_PNG.txt
    ../LICENSE_PortAudio.txt
    ../LICENSE_TIFF.txt
    ../LICENSE_zlib.txt
    ../README.txt)

file(GLOB tmp *.html)
set(doc ${doc} ${tmp})

file(GLOB tmp *.css)
set(doc ${doc} ${tmp})

file(GLOB tmp *.png)
set(doc ${doc} ${tmp})

file(GLOB tmp *.gif)
set(doc ${doc} ${tmp})

install(
    FILES ${doc}
    DESTINATION doc)

#install(
#    DIRECTORY doxygen
#    DESTINATION doc)
