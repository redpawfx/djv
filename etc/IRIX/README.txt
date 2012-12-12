--------------------------------------------------------------------------------
 SGI IRIX Package Creation
 
 References:
 
 * Silicon Graphics, "Software Packager User's Guide"
 
 Build the source code and install to a temporary directory.
 
 Build the package:
 
 > gendist -root $INSTALL_DIR -idb djv.idb -spec djv.spec -dist /tmp -verbose
 > cd /tmp
 > tar cvf djv-0.8.2_irix.tardist djv djv.idb djv.sw
 
 Default installation path: /usr/local/djv

