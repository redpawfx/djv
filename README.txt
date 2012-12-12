--------------------------------------------------------------------------------
 DJV Imaging - README
 
 DJV Imaging provides professional movie playback software for film
 production, VFX, and computer animation.

 Website:      http://djv.sourceforge.net
 E-Mail:       darbyjohnston@yahoo.com
 Mailing List: djv-users@lists.sourceforge.net
 
 Contents:
 
 * Installing the Software
   * Apple Mac OS
   * Linux
   * Microsoft Windows
   * Sun Solaris
 * Third-Party Software
 * Building Source Code
   * UNIX
   * Windows
 * Errata
 * Credits

--------------------------------------------------------------------------------
 Installing the Software, Apple Mac OS
 
 Requirements
 ------------
 
 * Apple Mac OS 10.5
   http://www.apple.com/macosx

 Download
 --------
 
 32-bit:
 
 * http://sourceforge.net/projects/djv/files/djv/djv-0.8.2_macos-x86.dmg/download
 
 Install
 -------
  
 Open the disk image and copy the package to where you would like to install it
 (a link to the Applications directory is provided as a shortcut).
 
 Post-Install
 ------------
 
 To use the command-line utilities add the install directory to your search
 path.

 Example C Shell ($HOME/.cshrc):

   setenv PATH ${PATH}:/Applications/djv-0.8.2.app/Contents/Resources/bin

 Example Bourne shell ($HOME/.profile):

   export PATH=$PATH:/Applications/djv-0.8.2.app/Contents/Resources/bin

 Uninstall
 ---------

 Delete the package from where you installed it.
 
--------------------------------------------------------------------------------
 Installing the Software, Linux

 Requirements
 ------------
  
 * CentOS 4.7
   http://www.centos.org
 * Ubuntu 8.04
   http://www.ubuntu.com

 Download
 --------
 
 32-bit:
 
 * Debian
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.2_linux-x86.deb/download
 * RPM
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.2_linux-x86.rpm/download
 * Tar
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.2_linux-x86.tar.gz/download
 
 64-bit:
 
 * Debian
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.2_linux-x64.deb/download
 * RPM
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.2_linux-x64.rpm/download
 * Tar
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.2_linux-x64.tar.gz/download

 Install
 -------
 
 Note that installing the software requires root permissions.
 
 Example Debian package install:
 
   > sudo dpkg -i djv_0.8.2_linux-x86.deb

 Example RPM package install:

   > sudo rpm -iv djv-0.8.2_linux-x86.rpm

 Post-Install
 ------------
 
 Add the install directory to your search path.
 
 Default install directory: /usr/local/djv
 
 Example C Shell ($HOME/.cshrc):

   setenv PATH ${PATH}:/usr/local/djv/bin

 Example Bourne shell ($HOME/.profile):

   export PATH=$PATH:/usr/local/djv/bin

 Uninstall
 ---------
 
 Example Debian package uninstall:
 
   > sudo dpkg --purge djv

 Example RPM package uninstall:
 
   > sudo rpm -e djv

--------------------------------------------------------------------------------
 Installing the Software, Microsoft Windows
 
 Requirements
 ------------
 
 * Microsoft Windows XP SP/2
   http://www.microsoft.com/windows

 Download
 --------
 
 32-bit:
 
 * Installer
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.3-pre2_winxp-x86.exe/download
 * ZIP
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.3-pre2_winxp-x86.zip/download

 64-bit:
 
 * Installer
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.3-pre2_winxp-x64.exe/download
 * ZIP
   http://sourceforge.net/projects/djv/files/djv/djv-0.8.3-pre2_winxp-x64.zip/download

--------------------------------------------------------------------------------
 Installing the Software, Sun Solaris

 Requirements
 ------------
 
 * Sun Solaris 10 - http://www.sun.com/solaris

 Download
 --------
 
 32-bit:
 
 * http://sourceforge.net/projects/djv/files/djv/djv-0.8.3-pre2_solaris-x86.gz/download
 
 64-bit:
 
 * http://sourceforge.net/projects/djv/files/djv/djv-0.8.3-pre2_solaris-x64.gz/download
 
 Install
 -------
 
 Note that installing the software requires root permissions.
 
 Example install:

   # gunzip djv-0.8.2_solaris-x86.gz
   # pkgadd -d djv-0.8.2_solaris-x86

 Post-Install
 ------------

 Add the install directory to your search path.
 
 Default install directory: /opt/djv
 
 Example C Shell ($HOME/.cshrc):

   setenv PATH ${PATH}:/opt/djv/bin

 Example Bourne shell ($HOME/.profile):

   export PATH=$PATH:/opt/djv/bin

 Uninstall
 ---------
 
 Example uninstall:
 
   # pkgrm djv
   
--------------------------------------------------------------------------------
 Third-Party Software

 Third-party software is included with the standard DJV packages except for
 OpenGL drivers and Apple QuickTime.

 Required software:
 
 * FLTK 1.1.9
   http://www.fltk.org
 * OpenGL 2.0 or 1.2
   http://www.opengl.org
 * GLEW 1.5.1
   http://glew.sourceforge.net
 * OpenEXR 1.6.1
   http://www.openexr.com

 Optional software:
 
 * Apple QuickTime 7.3 (Mac OS, Windows)
   http://www.apple.com/quicktime
 * FFmpeg
   http://ffmpeg.mplayerhq.hu
 * libjpeg 6b
   http://www.ijg.org
 * libpng 1.2.14
   http://www.libpng.org
 * libquicktime 1.1.0 (Linux, Solaris)
   http://libquicktime.sourceforge.net
 * libtiff 3.8.2
   http://www.libtiff.org
 * PortAudio
   http://www.portaudio.com

--------------------------------------------------------------------------------
 Building Source Code

 Third-party software is included with the source code, but if you would like
 to use your own versions you will need:
  
 Required:
 
 * CMake 2.6
   http://www.cmake.org
 * FLTK 1.1.9
   http://www.fltk.org
 * OpenGL 2.0 or 1.2
   http://www.opengl.org
 * GLEW 1.5.1
   http://glew.sourceforge.net
 * OpenEXR 1.6.1
   http://www.openexr.com

 Optional:
 
 * Apple QuickTime 7.3 (Mac OS, Windows)
   http://www.apple.com/quicktime
 * FFmpeg
   http://ffmpeg.mplayerhq.hu
 * libjpeg 6b
   http://www.ijg.org
 * libpng 1.2.14
   http://www.libpng.org
 * libquicktime 1.1.2 (Linux, Solaris)
   http://libquicktime.sourceforge.net
 * libtiff 3.8.2
   http://www.libtiff.org
 * PortAudio
   http://www.portaudio.com

--------------------------------------------------------------------------------
 Building Source Code, UNIX
 
 Download the source code:
   
   http://sourceforge.net/projects/djv/files
 
 Unpack the source code (referred to as $SOURCE):
 
   zcat djv-0.8.2_src.tar.gz | tar xvf -
 
 Check the CMake build configuration:
 
   $SOURCE/CMakeLists.txt
  
 To use system versions of third-party software instead of building them from
 source, uncomment this line:
 
   #set(djv_third_party_use_system true)

 Create a build directory (referred to as $BUILD) and run CMake from there:
 
   mkdir $BUILD
   cd $BUILD
   cmake -i $SOURCE
 
 Build:
 
   > make
 
 Run confidence tests:
 
   > make test

 Create an install package:
 
   > make package

--------------------------------------------------------------------------------
 Building Source Code, Windows
 
 Download the source code:
   
   http://sourceforge.net/projects/djv/files
 
 Unpack the source code (referred to as $SOURCE).
 
 Check the CMake build configuration:
 
   $SOURCE\CMakeLists.txt

 Create a build directory (referred to as $BUILD) and run CMake from there:
 
   mkdir $BUILD
   cd $BUILD
   cmake -G "Visual Studio 9 2008" $SOURCE
   
 Or for 64-bit support:
 
   cmake -G "Visual Studio 9 2008 Win64" $SOURCE
 
 TODO: Currently the Windows build directory must be called "djv", and be
 located in the same directory as the source directory. For example:
 
   C:\build\djv-0.8.2 - $SOURCE
   C:\build\djv       - $BUILD

 Build third-party software:
 
   $SOURCE\third_party\third_party.sln
 
 TODO: Currently builds only work in "Release" mode.
 
 Build DJV:
 
   $BUILD\djv.sln
 
 To run the binaries, add this directory to the search path:
 
   $BUILD\build\bin
 
 To build installable packages, you will need:
 
  * NSIS
    http://nsis.sourceforge.net
  * 7-Zip
    http://www.7-zip.org

 To create an install package, either build the target "PACKAGE" from
 within Visual Studio, or run the command "cpack" in the $BUILD directory.

--------------------------------------------------------------------------------
 Errata

 The OpenGL 1.2 renderer does not support the following features:

 * Floating-point images
 * Image rotation
 * Image filters

 OpenEXR:
 
 * OpenGL 2.0 is required for floating-point image support

--------------------------------------------------------------------------------
 Credits

 Darby Johnston, darbyjohnston@yahoo.com:
 
 * Original author
 
 Alan Jones, skyphyr@gmail.com:
 
 * Softimage XSI viewer wrapper

 Mikael Sundell, mikael.sundell@gmail.com:
 
 * IFF image file format support
 * Test images

