/*------------------------------------------------------------------------------
 lib/djv_ui/text_display.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "text.h"

#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Text_Display.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace text {

//------------------------------------------------------------------------------
// Text_Display::Widget
//------------------------------------------------------------------------------

class Text_Display::Widget : public Fl_Output
{
public:

  Widget() :
    Fl_Output(0, 0, 0, 0)
  {
    box(FL_FLAT_BOX);
    
    // XXX See Fl_Text_Display.cxx
    //scrollbar.slider(style::FL_BOX_THIN_UP);
    //hscrollbar.slider(style::FL_BOX_THIN_UP);
  }
};

//------------------------------------------------------------------------------
// Text_Multiline_Display::Widget
//------------------------------------------------------------------------------

class Text_Multiline_Display::Widget : public Fl_Text_Display
{
public:

  Widget() :
    Fl_Text_Display(0, 0, 0, 0)
  {
    buffer(&_value);

    box(FL_FLAT_BOX);
  }
  
  void set(const String & in)
  {
    _value.text(in.c_str());
  }

private:

  Fl_Text_Buffer _value;
};

//------------------------------------------------------------------------------
// Text_Display
//------------------------------------------------------------------------------

Text_Display::Text_Display(const String & in) :
  _value(in),
  _widget(0)
{
  debug_path("Text_Display");
  
  add(_widget = new Widget);

  widget_update();
}

Text_Display::~Text_Display()
{}

void Text_Display::set(const String & in)
{
  if (in == _value)
    return;
  
  DJV_DEBUG("Text_Display::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  _value = in;
  
  widget_update();
}
  
const String & Text_Display::get() const
{
  return _value;
}

void Text_Display::text_font(Fl_Font in)
{
  Text_Base::text_font(in);
  
  _widget->textfont(in);
}
  
Fl_Font Text_Display::text_font() const
{
  return Text_Base::text_font();
}

void Text_Display::text_color(Fl_Color in)
{
  Text_Base::text_color(in);
  
  _widget->textcolor(in);
}

Fl_Color Text_Display::text_color() const
{
  return Text_Base::text_color();
}

void Text_Display::size_string(const String & in)
{
  _size_string = in;
}
  
const String & Text_Display::size_string() const
{
  return _size_string;
}

void Text_Display::dirty()
{
  Text_Base::dirty();

  _widget->textfont(text_font());
  _widget->textsize(style::global()->font_size());
  _widget->textcolor(text_color());
  
  size_hint(
    V2i(
      //math::max(
      //  style::global()->size_button(),
      //  base::string_size(_size_string)
      //),
      base::string_width(_size_string),
      fl_height()
    ) +
    style::global()->margin_text() * 2 +
    frame_size() * 2
  );
}

void Text_Display::geom(const Box2i & in)
{
  Text_Base::geom(in);
  
  const Box2i & geom = frame_geom();
  
  _widget->resize(geom.x, geom.y, geom.w, geom.h);
}
  
const Box2i & Text_Display::geom() const
{
  return Text_Base::geom();
}

void Text_Display::widget_update()
{
  _widget->value(_value.c_str());
  _widget->position(0, 0);
}

//------------------------------------------------------------------------------
// Text_Multiline_Display
//------------------------------------------------------------------------------

Text_Multiline_Display::Text_Multiline_Display(const String & in) :
  _value(in),
  _word_wrap(false),
  _widget(0)
{
  debug_path("Text_Multiline_Display");
  
  // Create widgets.
  
  add(_widget = new Widget);
  
  // Initialize.
  
  widget_update();
}

Text_Multiline_Display::~Text_Multiline_Display()
{}

void Text_Multiline_Display::set(const String & in)
{
  if (in == _value)
    return;
  
  DJV_DEBUG("Text_Multiline_Display::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  _value = in;
  
  widget_update();
}

const String & Text_Multiline_Display::get() const
{
  return _value;
}

void Text_Multiline_Display::word_wrap(bool in)
{
  if (in == _word_wrap)
    return;
  
  _word_wrap = in;
  
  widget_update();
}

bool Text_Multiline_Display::word_wrap() const
{
  return _word_wrap;
}

void Text_Multiline_Display::text_font(Fl_Font in)
{
  Text_Base::text_font(in);
  
  _widget->textfont(in);
}
  
Fl_Font Text_Multiline_Display::text_font() const
{
  return Text_Base::text_font();
}

void Text_Multiline_Display::text_color(Fl_Color in)
{
  Text_Base::text_color(in);
  
  _widget->textcolor(in);
}

Fl_Color Text_Multiline_Display::text_color() const
{
  return Text_Base::text_color();
}

void Text_Multiline_Display::dirty()
{
  Text_Base::dirty();

  _widget->textfont(text_font());
  _widget->textsize(style::global()->font_size());
  _widget->textcolor(text_color());
  
  fl_font(text_font(), style::global()->font_size());
  
  const V2i size = base::string_size(String(20, '0'));
  
  size_hint(
    V2i(size.x, size.y * 8) +
    style::global()->margin_text() * 2 +
    frame_size() * 2
  );
}

void Text_Multiline_Display::geom(const Box2i & in)
{
  DJV_DEBUG("Text_Multiline_Display::geom");
  DJV_DEBUG_PRINT("in = " << in);
  
  const Box2i geom = this->geom();
  
  Text_Base::geom(in);

  if (geom != this->geom())
  {
    const Box2i & frame_geom = this->frame_geom();
  
    _widget->resize(frame_geom.x, frame_geom.y, frame_geom.w, frame_geom.h);
    
    widget_update();
  }
}
  
const Box2i & Text_Multiline_Display::geom() const
{
  return Text_Base::geom();
}

void Text_Multiline_Display::widget_update()
{
  _widget->set(
    _word_wrap ?
      string::join(
        string::lines(_value, frame_geom().w / base::char_width(), false),
        '\n'
      ) :
    _value
  );
}

//------------------------------------------------------------------------------

}
}
