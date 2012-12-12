/*------------------------------------------------------------------------------
 lib/djv_base/file_split.cxx

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
// split()
//------------------------------------------------------------------------------

namespace {

// XXX

inline bool _path_separator(char in)
{
  return in == path_separator_list[0] || in == path_separator_list[1];
}

inline bool _seq_valid(char in)
{
  return string::is_digit(in) || '#' == in;
}

inline bool _seq_separator(char in)
{
  return '-' == in || ',' == in;
}

inline bool _pad_match(const String & a, const String & b)
{
  DJV_DEBUG("_pad_match");
  DJV_DEBUG_PRINT("a = " << a);
  DJV_DEBUG_PRINT("b = " << b);
  
  return ((a.size() > 1 && '0' == a[0]) || (b.size() > 1 && '0' == b[0])) ?
    (a.size() == b.size()) :
    true;
}

}

void split(
  const String & in,
  String & path,
  String & base,
  String & number,
  String & extension
) {
  DJV_DEBUG("split");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("size = " << in.size());
  
  path.resize(0);
  base.resize(0);
  number.resize(0);
  extension.resize(0);

  const int size = static_cast<int>(in.size());
  if (! size)
    return;

  // Extension.

  int i = size - 1;
  int tmp = i;
  for (; in[i] != '.' && ! _path_separator(in[i]) && i > 0; --i) ;
  if (i > 0 && '.' == in[i])
  {
    extension = String(in, i, tmp - i + 1);
    --i;
  }
  else
  {
    i = size - 1;
  }
  
  DJV_DEBUG_PRINT("extension = " << extension);

  // Number.

  if (i >= 0 && _seq_valid(in[i]))
  {
    tmp = i;
    int separator = -1;
    String word;
    for (; i > 0; --i)
    {
      if (! _seq_valid(in[i - 1]) || _seq_separator(in[i - 1]))
      {
        if (
          separator != -1 &&
          ! _pad_match(String(in, i, separator - i), word)
        ) {
          i = separator + 1;
          break;
        }
        else
        {
          word = String(
            in,
            i,
            -1 == separator ? (tmp - i + 1) : (separator - i)
          );
          
          separator = i - 1;
        }
      }
      if (! (_seq_valid(in[i - 1]) || _seq_separator(in[i - 1])))
        break;
    }
    number = String(in, i, tmp - i + 1);
    --i;
  }
  
  DJV_DEBUG_PRINT("number = " << number);

  // Base.
  
  if (i >= 0 && ! _path_separator(in[i]))
  {
    tmp = i;
    for (; i > 0 && ! _path_separator(in[i - 1]); --i) ;
    base = String(in, i, tmp - i + 1);
    --i;
  }
  
  DJV_DEBUG_PRINT("base = " << base);

  // Path.

  if (i >= 0)
    path = String(in, 0, i + 1);
  
  DJV_DEBUG_PRINT("path = " << path);
}

//------------------------------------------------------------------------------

}
}
