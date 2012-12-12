/*------------------------------------------------------------------------------
 bin/djv_info/command_line.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include "etc.h"
#include <djv_image/io.h>

namespace djv_bin {
namespace info {

//------------------------------------------------------------------------------
// Application::command_line()
//------------------------------------------------------------------------------

void Application::command_line(String & in) throw (Error)
{
  djv_image::application::Application_Base::command_line(in);

  if (exit() != djv_base::application::EXIT_DEFAULT)
    return;
  
  if (in.empty())
  {
    exit(djv_base::application::EXIT_HELP);
    return;
  }

  String arg;
  try
  {
    while (! in.empty())
    {
      in >> arg;
    
      // Options.
    
      if (
        string::compare_no_case("-x_info", arg) ||
        string::compare_no_case("-i", arg)
      ) {
        _info = false;
      }
      else if (
        string::compare_no_case("-verbose", arg) ||
        string::compare_no_case("-v", arg)
      ) {
        _verbose = true;
      }
      else if (
        string::compare_no_case("-file_path", arg) ||
        string::compare_no_case("-fp", arg)
      ) {
        _file_path = true;
      }
      else if (
        string::compare_no_case("-seq", arg) ||
        string::compare_no_case("-q", arg)
      ) {
        in >> _seq;
      }
      else if (
        string::compare_no_case("-recurse", arg) ||
        string::compare_no_case("-r", arg)
      ) {
        _recurse = true;
      }
      else if (
        string::compare_no_case("-columns", arg) ||
        string::compare_no_case("-c", arg)
      ) {
        in >> _columns;
      }
    
      // Arguments.
    
      else
      {
        _input += arg;
      }
    }
  }
  catch (String)
  {
    throw Error(arg);
  }

  if (! _input.size())
    throw Error(error_command_line_input);
}

//------------------------------------------------------------------------------
// Application::command_line_help()
//------------------------------------------------------------------------------

const String label_command_line_help =
"%%\n"
" djv_info\n"
"\n"
" Utility for displaying image information.\n"
"\n"
" Example output:\n"
"\n"
"   yesterdayview.mov                      640x424:1.51 RGB U8 00:02:00:01@12\n"
"   dlad.dpx                            2048x1556:1.32 RGB U10 00:00:00:01@24\n"
"   render0001-1000.exr                                        00:00:41:16@24\n"
"     0: A,B,G,R                                         720x480:1.5 RGBA F16\n"
"     1: Ambient                                          720x480:1.5 RGB F16\n"
"     2: Diffuse                                          720x480:1.5 RGB F16\n"
"     3: Specular                                         720x480:1.5 RGB F16\n"
"     4: Z                                                  720x480:1.5 L F32\n"
"\n"
" Key:\n"
"\n"
"   (name) (width)x(height):(aspect) (format) (type) (duration)@(speed)\n"
"\n"
" Layer key:\n"
"\n"
"   (name) (duration)@(speed)\n"
"     (layer): (name) (width)x(height):(aspect) (format) (type)\n"
"\n"
"%%\n"
" Usage\n"
"\n"
"   djv_info (image)... (directory)... [option]...\n"
"\n"
" Command line options are case-insensitive.\n"
"\n"
"%%\n"
" Options\n"
"\n"
" Don't show image information, only file names:\n"
"\n"
"   -x_info, -i\n"
"\n"
" Show verbose image information:\n"
"\n"
"   -verbose, -v\n"
"\n"
" Show file path names:\n"
"\n"
"   -file_path, -fp\n"
"\n"
" File sequencing:\n"
"\n"
"   -seq, -q (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
" Descend into sub-directories:\n"
"\n"
"   -recurse, -r\n"
"\n"
" Number of columns for formatting output:\n"
"\n"
"   -columns, -c (value)\n"
"\n"
"   Values = 0 (disable formatting) - maximum\n"
"\n"
"%%"
"%%\n"
" Examples\n"
"\n"
" Display image information:\n"
"\n"
"   > djv_info image.sgi image2.sgi\n"
"\n"
" Display image sequence information:\n"
"\n"
"   > djv_info image.1-100.sgi\n"
"\n"
" Display information about all images within a directory:\n"
"\n"
"   > djv_info ~/pics\n"
"\n"
" Display video lookup table information:\n"
"\n"
"   > djv_info 0.vlut\n"
"\n";

String Application::command_line_help() const
{
  return _help_default ?
    String(string::Format(label_command_line_help).
      arg(command_line_separator()).
      arg(command_line_separator()).
      arg(command_line_separator()).
      arg(string::lower(file_seq::label_compress()), ", ").
      arg(string::lower(string::label(_seq))).
      arg(djv_image::application::Application_Base::command_line_help()).
      arg(command_line_separator())) :
    djv_image::application::Application_Base::command_line_help();
}

//------------------------------------------------------------------------------

}
}

