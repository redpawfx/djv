/*------------------------------------------------------------------------------
 bin/djv_ls/command_line.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include "etc.h"

namespace djv_bin {
namespace ls {

//------------------------------------------------------------------------------
// Application::command_line()
//------------------------------------------------------------------------------

void Application::command_line(String & in) throw (Error)
{
  application::Application_Base::command_line(in);
  
  if (exit() != application::EXIT_DEFAULT)
    return;
  
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
      else if (string::compare_no_case("-hidden", arg))
      {
        _hidden = true;
      }
      else if (
        string::compare_no_case("-filter", arg) ||
        string::compare_no_case("-f", arg)
      ) {
        in >> _filter;
      }
      else if (
        string::compare_no_case("-columns", arg) ||
        string::compare_no_case("-c", arg)
      ) {
        in >> _columns;
      }
    
      // Sorting options.
    
      else if (
        string::compare_no_case("-sort", arg) ||
        string::compare_no_case("-s", arg)
      ) {
        in >> _sort;
      }
      else if (
        string::compare_no_case("-sort_reverse", arg) ||
        string::compare_no_case("-sr", arg)
      ) {
        _sort_reverse = true;
      }
      else if (
        string::compare_no_case("-x_sort_dir", arg) ||
        string::compare_no_case("-sd", arg)
      ) {
        _x_sort_directory = false;
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
}

//------------------------------------------------------------------------------
// Application::command_line_help()
//------------------------------------------------------------------------------

const String label_command_line_help =
"%%\n"
" djv_ls\n"
"\n"
" Utility for listing directories.\n"
"\n"
" Example output:\n"
"\n"
"   el_cerrito_bart.1k.tiff     File 2.23MB darby rw Mon Jun 12 21:21:55 2006\n"
"   richmond_train.2k.tiff      File 8.86MB darby rw Mon Jun 12 21:21:58 2006\n"
"   fishpond.1-749.png         Seq 293.17MB darby rw Thu Aug 17 16:47:43 2006\n"
"\n"
" Key:\n"
"\n"
"   (name) (size) (user) (permissions) (time)\n"
"\n"
" File types:\n"
"\n"
" * file - Regular file\n"
" * seq - File sequence\n"
" * dir - Directory\n"
"\n"
" Permissions:\n"
"\n"
" * r - Readable\n"
" * w - Writable\n"
" * x - Executable\n"
"\n"
"%%\n"
" Usage\n"
"\n"
"   djv_ls [file]... [directory]... [option]...\n"
"\n"
" Command line options are case-insensitive.\n"
"\n"
"%%\n"
" Options\n"
"\n"
" Don't show information, only file names:\n"
"\n"
"   -x_info, -i\n"
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
" Show hidden files:\n"
"\n"
"   -hidden\n"
"\n"
" Filter files (e.g., *, ?):\n"
"\n"
"   -filter, -f (value)\n"
"\n"
" Number of columns for formatting output:\n"
"\n"
"   -columns, -c (value)\n"
"\n"
"   Values = 0 (disable formatting) - maximum\n"
"\n"
"%%\n"
" Sorting Options\n"
"\n"
" Sorting:\n"
"\n"
"   -sort, -s (value)\n"
"\n"
"   Values = %%\n"
"\n"
"   Default = %%\n"
"\n"
" Reverse sort order:\n"
"\n"
"   -sort_reverse, -sr\n"
"\n"
" Don't sort directories first:\n"
"\n"
"   -x_sort_dir, -sd\n"
"\n"
"%%"
"%%\n"
" Examples\n"
"\n"
" List the current directory:\n"
"\n"
"   > djv_ls\n"
"\n"
" List specific directories:\n"
"\n"
"   > djv_ls ~/movies ~/pictures\n"
"\n"
" Sort by time with the most recent first:\n"
"\n"
"   > djv_ls -sort time -sort_reverse\n"
"\n"
" Show only files ending with .tiff or .tif (note the use of double-quotes):\n"
"\n"
"   > djv_ls -filter \"*.tiff\" -filter \"*.tif\"\n"
"\n";

String Application::command_line_help() const
{
  return string::Format(label_command_line_help).
    arg(command_line_separator()).
    arg(command_line_separator()).
    arg(command_line_separator()).
    arg(string::lower(file_seq::label_compress()), ", ").
    arg(string::lower(string::label(_seq))).
    arg(command_line_separator()).
    arg(string::lower(file::label_sort()), ", ").
    arg(string::lower(string::label(_sort))).
    arg(application::Application_Base::command_line_help()).
    arg(command_line_separator());
}

//------------------------------------------------------------------------------

}
}

