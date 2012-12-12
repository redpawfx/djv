/*------------------------------------------------------------------------------
 lib/djv_base/string_format.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "string.h"

#include <djv_base/debug.h>
#include <djv_base/string.h>

namespace djv_base {
namespace string {

//------------------------------------------------------------------------------
// Format
//------------------------------------------------------------------------------

Format::Format(const String & in) :
  _value(in)
{}

Format Format::arg(const String & in, int width, char fill) const
{
  //DJV_DEBUG("Format::arg");
  //DJV_DEBUG_PRINT("value = " << _value);
  //DJV_DEBUG_PRINT("in = " << in << " (" << in.size() << ")");
  //DJV_DEBUG_PRINT("width = " << width);
  //DJV_DEBUG_PRINT("fill = " << fill);
  
  // Find argument string "%%".

  int size = static_cast<int>(_value.size()), index = -1;
  for (int i = 0; i < size - 1; ++i)
    if ('%' == _value[i] && '%' == _value[i + 1])
    {
      index = i;
      break;
    }
  if (-1 == index) return _value;
  //DJV_DEBUG_PRINT("index = " << index);
  
  // Format input.

  String tmp = in;
  if (width)
  {
    int w = (width < 0 ? -width : width) - static_cast<int>(in.size());
    if (w < 0) w = 0;
    if (width < 0) tmp += String(w, fill);
    else tmp.insert(tmp.begin(), w, fill);
  }
  
  // Replace.
  
  const String out = String(_value, 0, index) + tmp +
    String(_value, index + 2, _value.size() - (index + 2));
  //DJV_DEBUG_PRINT("out = " << out);
  return out;
}

Format Format::arg(const List<String> & in, const String & separator, int width,
  char fill) const
{
  return arg(join(in, separator), width, fill);
}

Format Format::arg(int in, int width, char fill) const
{
  return arg(int_to_string(in), width, fill);
}

Format Format::arg(int64_t in, int width, char fill) const
{
  return arg(int_to_string(in), width, fill);
}

Format Format::arg(uint in, int width, char fill) const
{
  return arg(int_to_string(in), width, fill);
}

Format Format::arg(uint64_t in, int width, char fill) const
{
  return arg(int_to_string(in), width, fill);
}

Format Format::arg(double in, int width, int precision, char fill) const
{
  return arg(float_to_string(in, precision), width, fill);
}

Format::operator String () const
{
  return _value;
}

//------------------------------------------------------------------------------

}
}

