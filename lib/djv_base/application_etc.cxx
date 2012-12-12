/*------------------------------------------------------------------------------
 lib/djv_base/application_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "application.h"

namespace djv_base {
namespace application {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String label_info =
" General:\n"
"\n"
" * Version: %%\n"
" * System: %%-%%\n"
" * System string: %%\n"
" * Endian: %%\n"
" * Search path: %%\n";

extern const String label_about =
" DJV Imaging %%\n"
" http://djv.sourceforge.net\n"
" Made in Berkeley, CA, USA\n"
" Copyright (c) 2004-2009 Darby Johnston\n"
" All rights reserved.\n"
"\n"
" Redistribution and use in source and binary forms, with or without"
" modification, are permitted provided that the following conditions are met:\n"
"\n"
" * Redistributions of source code must retain the above copyright notice,"
" this list of conditions, and the following disclaimer.\n"
"\n"
" * Redistributions in binary form must reproduce the above copyright notice,"
" this list of conditions, and the following disclaimer in the documentation"
" and/or other materials provided with the distribution.\n"
"\n"
" * Neither the names of the copyright holders nor the names of any"
" contributors may be used to endorse or promote products derived from this"
" software without specific prior written permission.\n"
"\n"
" THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS"
" \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED"
" TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR"
" PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR"
" CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,"
" EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,"
" PROCUREMENT OF SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;"
" OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,"
" WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR"
" OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF"
" ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
"\n"
" Portions of this software are copyright (in alphabetical order):\n"
"\n"
" Copyright (c) 2008 Alan Jones.\n"
" All rights reserved.\n"
"\n"
" Copyright (c) 2008-2009 Mikael Sundell\n"
" All rights reserved.\n"
"\n"
" This software is based in part on the works of (in alphabetical order):\n"
"\n"
" CTL\n"
" http://www.oscars.org/science-technology/council/projects/ctl.html\n"
" Copyright (c) 2007 Academy of Motion Picture Arts and Sciences (A.M.P.A.S.)\n"
" Portions contributed by others as indicated. All rights reserved.\n"
"\n"
" FFmpeg\n"
" http://ffmpeg.mplayerhq.hu\n"
" Copyright (c) 2000-2003 Fabrice Bellard\n"
" LGPL License http://www.gnu.org/licenses/lgpl.html\n"
"\n"
" FLTK\n"
" http://www.fltk.org\n"
" Copyright 1998-2006, Bill Spitzak\n"
"\n"
" GLEW\n"
" http://glew.sourceforge.net\n"
" Copyright (c) 2002-2007, Milan Ikits <milan ikits[]ieee org>\n"
" Copyright (c) 2002-2007, Marcelo E. Magallon <mmagallo[]debian org>\n"
" Copyright (c) 2002, Lev Povalahev\n"
" All rights reserved.\n"
"\n"
" libjpeg\n"
" http://www.ijg.org\n"
" Copyright (c) 1991-1998, Thomas G. Lane\n"
"\n"
" libpng\n"
" http://www.libpng.org\n"
" Copyright (c) 2004, 2006 Glenn Randers-Pehrson\n"
"\n"
" libquicktime\n"
" http://libquicktime.sourceforge.net\n"
" Copyright (c) 2002 Heroine Virtual Ltd.\n"
" Copyright (c) 2002-2007 Members of the libquicktime project\n"
" LGPL License http://www.gnu.org/licenses/lgpl.html\n"
"\n"
" libtiff\n"
" http://www.libtiff.org\n"
" Copyright (c) 1988-1997 Sam Leffler\n"
" Copyright (c) 1991-1997 Silicon Graphics, Inc.\n"
"\n"
" Mesa\n"
" http://www.mesa3d.org\n"
" Copyright (c) 1999-2007 Brian Paul\n"
"\n"
" OpenEXR\n"
" http://www.openexr.com\n"
" Copyright (c) 2004, Industrial Light & Magic, a division of Lucasfilm"
" Entertainment Company Ltd.\n"
"\n"
" PortAudio\n"
" http://www.portaudio.com\n"
" Copyright (c) 1999-2006 Phil Burk and Ross Bencina\n"
"\n"
" Qt\n"
" http://www.qtsoftware.com\n"
" Copyright (c) 2008-2009 Nokia Corporation\n"
"\n"
" zlib\n"
" http://www.zlib.net/\n"
" Copyright (c) 1995-2005 Jean-loup Gailly and Mark Adler\n";

extern const String label_command_line_help =
"%%\n"
" General Options\n"
"\n"
" Show help message:\n"
"\n"
"   -help, -h\n"
"\n"
" Show information message:\n"
"\n"
"   -info\n"
"\n"
" Show about message:\n"
"\n"
"   -about\n"
"\n";

extern const String
  error_command_line = "Cannot parse the command line argument: %%";

//------------------------------------------------------------------------------

}
}

