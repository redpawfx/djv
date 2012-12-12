/*------------------------------------------------------------------------------
 include/djv_base/file_seq_fnc_inline.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace file_seq {

//------------------------------------------------------------------------------
// string_to_frame()
//------------------------------------------------------------------------------

inline int64_t string_to_frame(const String & in, int * pad)
{
  const String::size_type size = in.size();

  if (size)
    if ('#' == in[0])
      return -1;

  if (pad)
  {
    *pad = 0;
    if (size > 1)
      if ('0' == in[0])
        *pad = static_cast<int>(size);
  }

  return string::string_to_int64(in);
}

//------------------------------------------------------------------------------

}
}

