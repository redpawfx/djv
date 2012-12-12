/*------------------------------------------------------------------------------
 lib/djv_base/file_path.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file.h"

#include <FL/filename.H>

namespace djv_base {
namespace file {

//------------------------------------------------------------------------------
// path_absolute()
//------------------------------------------------------------------------------

String path_absolute(const String & in)
{
  DJV_DEBUG("path_absolute");
  DJV_DEBUG_PRINT("in = " << in);
  
  char * p = 0;
  
  char tmp [string::cstring_size] = "";
  fl_filename_absolute(tmp, string::cstring_size, in.c_str());
  p = tmp;
  
  // XXX

#if defined(DJV_WINDOWS)

  TCHAR tmp2 [4096];
  GetFullPathName(p, 4096, tmp2, 0);
  p = tmp2;

#endif // DJV_WINDOWS

  String out(p);
  DJV_DEBUG_PRINT("out = " << out);
  return out;
}

//------------------------------------------------------------------------------
// path_is_absolute()
//------------------------------------------------------------------------------

bool path_is_absolute(const String & in)
{
  if (! in.size()) return false;

  // XXX Windows drive letter.
  
  if (1 == in.find(':', 0)) return true;

  return list::find(in[0], path_separator_list) != -1;
}

//------------------------------------------------------------------------------
// path_fix()
//------------------------------------------------------------------------------

String path_fix(const String & in)
{
  DJV_DEBUG("path_fix");
  DJV_DEBUG_PRINT("in = " << in);

  // Let FLTK do it's stuff.
  
  char tmp [string::cstring_size] = "";
  char path [string::cstring_size] = "";
  fl_filename_expand(tmp, string::cstring_size, in.c_str());
  fl_filename_absolute(path, string::cstring_size, tmp);

  // XXX Keep original path separators?

  //for (char * p = path; *p && (p - path) < string::cstring_size; ++p)
  //  if (path_separator_list[0] == *p)
  //    *p = path_separator[0];
  
  DJV_DEBUG_PRINT("path = " << path);
  
  // Split path into pieces.
  
  List<String> list = string::split(path, path_separator_list);
  
  // XXX Keep extra root piece for Windows.

  String root;
  if (
    (path_separator_list[0] == path[0] && path_separator_list[0] == path[1]) ||
    (path_separator_list[1] == path[0] && path_separator_list[1] == path[1])
  ) {
    root = path_separator;
  }
  else if (list.size())
  {
    if (list[0].find(':', 0) != String::npos)
    {
      root = list[0];
      list.erase(list.begin());
    }
  }

  // Remove extra pieces.
  
  List<String> list_tmp;
  for (List<String>::const_iterator i = list.begin(); i < list.end(); ++i)
    if (".." == *i && list_tmp.size())
    {
      if (list_tmp[list_tmp.size() - 1] != "..")
        list_tmp.erase(list_tmp.end() - 1);
      else
        list_tmp += *i;
    }
    else if ("." == *i && list_tmp.size()) ;
    else list_tmp += *i;
  
  // Combine pieces.

  String out = root + path_separator + string::join(list_tmp, path_separator);
  if (list_tmp.size()) out += path_separator;
  DJV_DEBUG_PRINT("out = " << out);
  return out;
}

//------------------------------------------------------------------------------
// path_posix()
//------------------------------------------------------------------------------

/*String path_posix(const String & in)
{
  String out = in;
  for (String::size_type i = 0; i < out.size(); ++i)
    if (path_separator_list[1] == out[i])
      out[i] = path_separator_list[0];
  return out;
}*/

//------------------------------------------------------------------------------

}
}

