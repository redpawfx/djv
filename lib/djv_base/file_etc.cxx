/*------------------------------------------------------------------------------
 lib/djv_base/file_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file.h"

namespace djv_base {
namespace file {

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(TYPE, label_type())
_DJV_STRING_OPERATOR_LABEL(SORT, label_sort())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_type()
{
  static const List<String> data = List<String>() <<
    "File" <<
    "Seq" <<
    "Dir";
  
  DJV_ASSERT(data.size() == _TYPE_SIZE);

  return data;
}

String label_size(uint64_t in)
{
  if (in > memory::terabyte)
  {
    return string::Format("%%TB").arg(in /
      static_cast<double>(memory::terabyte), 0, -2);
  }
  else if (in > memory::gigabyte)
  {
    return string::Format("%%GB").arg(in /
      static_cast<double>(memory::gigabyte), 0, -2);
  }
  else if (in > memory::megabyte)
  {
    return string::Format("%%MB").arg(in /
      static_cast<double>(memory::megabyte), 0, -2);
  }
  else
  {
    return string::Format("%%KB").arg(in /
      static_cast<double>(memory::kilobyte), 0, -2);
  }
}

const List<String> & label_perm()
{
  static const List<String> data = List<String>() <<
    "r" <<
    "w" <<
    "X";
  
  DJV_ASSERT(data.size() == _PERM_SIZE);
  
  return data;
}

const String & label_perm(int in)
{
  static const String data [] =
  {
    "",
    "r",
    "w",
    "rw",
    "x",
    "rx",
    "wx",
    "rwx"
  };
  
  return data[in];
}

const List<String> & label_sort()
{
  static const List<String> data = List<String>() <<
    "Name" <<
    "Type" <<
    "Size" <<
    "User" <<
    "Permissions" <<
    "Time";
  
  DJV_ASSERT(data.size() == _SORT_SIZE);
  
  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, const File & in)
{
  return debug << in.get();
}

//------------------------------------------------------------------------------

}
}

