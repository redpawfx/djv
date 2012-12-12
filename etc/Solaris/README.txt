--------------------------------------------------------------------------------
 Sun Solaris Package Creation
 
 References:
 
 * Sun Microsystems, "Application Packaging Developer's Guide",
   http://docs.sun.com/app/docs/doc/805-6338
 
 Build the source code and install to a temporary directory.

 Build the package:
 
 > pkgmk -r $INSTALL_DIR
 > pkgtrans -s /var/spool/pkg djv-0.8.2_solaris-sparc djv
 > gzip /var/spool/pkg/djv-0.8.2_solaris-sparc
 
 Default installation path: /opt/djv

