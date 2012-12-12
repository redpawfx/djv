/*------------------------------------------------------------------------------
 lib/djv_ui/base_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "base.h"

#include <djv_base/math.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace base {

//------------------------------------------------------------------------------
// cursor()
//------------------------------------------------------------------------------

void cursor(const Fl_Cursor & in)
{
  fl_cursor(in);

  // XXX
  
  Fl::flush();
}

//------------------------------------------------------------------------------
// char_size(), char_width()
//------------------------------------------------------------------------------

namespace {

// XXX

/*int char_average_size()
{
  double out = 0.0;
  
  int count = 0;
  for (int i = 32; i <= 126; ++i, ++count)
    out += fl_width(static_cast<uchar>(i));
  
  return math::ceil(out / static_cast<double>(count));
}*/

int char_average_size()
{
  return math::ceil(fl_width('0') * 0.9);
}

}

V2i char_size()
{
  return V2i(char_average_size(), fl_height());
}

int char_width()
{
  return char_average_size();
}

int char_width(char in)
{
  return static_cast<int>(fl_width(in));
}

//------------------------------------------------------------------------------
// string_size(), string_width()
//------------------------------------------------------------------------------

V2i string_size(const String & in)
{
  DJV_DEBUG("string_size");
  DJV_DEBUG_PRINT("in = " << in);
  
  const List<String> lines = string::split(in, '\n', true);
  
  DJV_DEBUG_PRINT("lines = " << lines.size());

  int width = 0;
  for (list::size_type i = 0; i < lines.size(); ++i)
    width = math::max(string_width(lines[i]), width);

  const V2i out(width, static_cast<int>(lines.size()) * fl_height());
  
  DJV_DEBUG_PRINT("out = " << out);
  
  return out;
}

int string_width(const String & in)
{
  return math::ceil(fl_width(in.c_str()));
}

int string_width(int char_size)
{
  return char_width() * char_size;
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  tooltip_reset = "Reset value";

//------------------------------------------------------------------------------

}
}
