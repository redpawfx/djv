/*------------------------------------------------------------------------------
 lib/djv_base/file_filter.cxx

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
// path_is_relative()
//------------------------------------------------------------------------------

namespace {

inline bool path_is_relative(const String & in)
{
  if ("." == in) ;
  else if (".." == in) ;
  else if (in.size() >= 2 && '.' == in[0] && ('/' == in[1] || '\\' == in[1])) ;
  else if (in.size() >= 3 && '.' == in[0] && '.' == in[1] &&
    ('/' == in[2] || '\\' == in[2])) ;
  else return false;
  return true;
}

}

//------------------------------------------------------------------------------
// filter()
//------------------------------------------------------------------------------

void filter(
  List<File> & in,
  int filter,
  const List<String> & glob,
  bool glob_files_only
) {
  //DJV_DEBUG("filter");
  //DJV_DEBUG_PRINT("filter = " << filter);
  //DJV_DEBUG_PRINT("glob = " << glob);
  
  list::size_type i = 0;
  for (list::size_type j = 0; j < in.size(); ++j)
  {
    const String name = in[j].get(-1, false);
    
    //DJV_DEBUG_PRINT("name = " << name);
    
    bool valid = true;
    
    const TYPE type = in[j].type();
    if ((filter & FILTER_FILE) && (FILE == type || SEQ == type))
      valid = false;
    if ((filter & FILTER_DIRECTORY) && DIRECTORY == type)
      valid = false;
    if ((filter & FILTER_HIDDEN) && '.' == name[0] && ! path_is_relative(name))
      valid = false;
    
    if (
      glob.size() &&
      ((glob_files_only && type != DIRECTORY) || ! glob_files_only)
    ) {
      list::size_type k = 0;
      for (; k < glob.size(); ++k)
        if (fl_filename_match(name.c_str(), glob[k].c_str())) break;
      if (glob.size() == k)
        valid = false;
    }
    
    //DJV_DEBUG_PRINT("valid = " << valid);
    
    if (valid)
    {
      if (i != j)
        in[i] = in[j];
      ++i;
    }
  }
  
  //DJV_DEBUG_PRINT("size = " << i);
  
  in.resize(i);
}

//------------------------------------------------------------------------------

}
}

