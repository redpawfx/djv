/*------------------------------------------------------------------------------
 lib/djv_ui/style_etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "style.h"

#include <FL/fl_draw.H>

namespace djv_ui {
namespace style {

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

extern const Fl_Color FL_COLOR_BUTTON =
  static_cast<Fl_Color>(FL_FREE_COLOR + 0);
extern const Fl_Color FL_COLOR_HIGHLIGHT =
  static_cast<Fl_Color>(FL_FREE_COLOR + 1);
extern const Fl_Color FL_COLOR_BORDER =
  static_cast<Fl_Color>(FL_FREE_COLOR + 2);

extern const Fl_Boxtype FL_BOX_UP =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 0);
extern const Fl_Boxtype FL_BOX_UP_H1 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 1);
extern const Fl_Boxtype FL_BOX_UP_H2 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 2);
extern const Fl_Boxtype FL_BOX_UP_H3 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 3);
extern const Fl_Boxtype FL_BOX_UP_V1 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 4);
extern const Fl_Boxtype FL_BOX_UP_V2 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 5);
extern const Fl_Boxtype FL_BOX_UP_V3 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 6);

extern const Fl_Boxtype FL_BOX_DOWN =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 7);
extern const Fl_Boxtype FL_BOX_DOWN_H1 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 8);
extern const Fl_Boxtype FL_BOX_DOWN_H2 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 9);
extern const Fl_Boxtype FL_BOX_DOWN_H3 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 10);
extern const Fl_Boxtype FL_BOX_DOWN_V1 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 11);
extern const Fl_Boxtype FL_BOX_DOWN_V2 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 12);
extern const Fl_Boxtype FL_BOX_DOWN_V3 =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 13);

extern const Fl_Boxtype FL_BOX_THIN_UP =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 14);
extern const Fl_Boxtype FL_BOX_THIN_DOWN =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 15);

extern const Fl_Boxtype FL_BOX_BORDER =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 16);

extern const Fl_Boxtype FL_BOX_MENU =
  static_cast<Fl_Boxtype>(FL_FREE_BOXTYPE + 17);

//------------------------------------------------------------------------------
// fltk_to_color()
//------------------------------------------------------------------------------

djv_image::Color fltk_to_color(Fl_Color in)
{
  djv_image::Color out(djv_image::pixel::RGB_U8);
  
  const Color & color_value = global()->color_value();
  djv_image::Color tmp;
  
  switch (in)
  {
    case FL_FOREGROUND_COLOR: tmp = color_value.foreground; break;
    case FL_BACKGROUND_COLOR: tmp = color_value.background; break;
    case FL_BACKGROUND2_COLOR: tmp = color_value.background2; break;
    case FL_SELECTION_COLOR: tmp = color_value.select; break;
    case FL_COLOR_BUTTON: tmp = color_value.button; break;
    case FL_COLOR_HIGHLIGHT: tmp = color_value.highlight; break;
  }
  
  djv_image::color::convert(tmp, out);
  
  return out;
}

//------------------------------------------------------------------------------
// color_dark(), color_light()
//------------------------------------------------------------------------------

void color_dark(Fl_Color in)
{
  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  djv_image::color::scale(0.8, style::fltk_to_color(in), tmp);
  fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
}

void color_light(Fl_Color in)
{
  djv_image::Color tmp(djv_image::pixel::RGB_U8);
  djv_image::color::scale(1.2, style::fltk_to_color(in), tmp);
  fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Color & a, const Color & b)
{
  return
    a.foreground == b.foreground &&
    a.background == b.background &&
    a.background2 == b.background2 &&
    a.button == b.button &&
    a.select == b.select &&
    a.highlight == b.highlight;
}

bool operator != (const Color & a, const Color & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(COLOR, label_color())

String & operator >> (String & in, Color & out) throw (String)
{
  in >> out.foreground;
  in >> out.background;
  in >> out.background2;
  in >> out.button;
  in >> out.select;
  in >> out.highlight;
  return in;
}

String & operator << (String & out, const Color & in)
{
  out << in.foreground;
  out << in.background;
  out << in.background2;
  out << in.button;
  out << in.select;
  out << in.highlight;
  return out;
}

_DJV_STRING_OPERATOR_LABEL(SIZE, label_size())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_color()
{
  static const List<String> data = List<String>() <<
    "Dark" <<
    "Neutral" <<
    "Light" <<
    "Custom";
  
  DJV_ASSERT(data.size() == _COLOR_SIZE);
  
  return data;
}

const List<String> & label_size()
{
  static const List<String> data = List<String>() <<
    "Small" <<
    "Medium" <<
    "Large" <<
    "Custom";
  
  DJV_ASSERT(data.size() == _SIZE_SIZE);
  
  return data;
}

//------------------------------------------------------------------------------

}
}

