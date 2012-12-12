/*------------------------------------------------------------------------------
 lib/djv_ui/text_edit.cxx

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
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace text {

//------------------------------------------------------------------------------
// _Input
//------------------------------------------------------------------------------

namespace {

struct _Input : public Fl_Input
{
  _Input(Text_Base * parent) :
    Fl_Input(0, 0, 0, 0),
    _parent(parent)
  {
    when(FL_WHEN_CHANGED);

    box(FL_FLAT_BOX);
  }
  
  int handle(int in)
  {
    switch (in)
    {
      case FL_FOCUS:
        _parent->draw_select(true);
        break;

      case FL_UNFOCUS:
        _parent->draw_select(false);
        break;
    }
    
    return Fl_Input::handle(in);
  }

private:

  Text_Base * _parent;
};

}

//------------------------------------------------------------------------------
// Text_Edit::_Text_Edit
//------------------------------------------------------------------------------

struct Text_Edit::_Text_Edit
{
  _Input * widget;
};

//------------------------------------------------------------------------------
// Text_Edit
//------------------------------------------------------------------------------

Text_Edit::Text_Edit(const String & in) :
  signal(this),
  action_signal(this),
  change_signal(this),
  _value(in),
  _p(new _Text_Edit)
{
  debug_path("Text_Edit");
  
  // Create widgets.
  
  add(_p->widget = new _Input(this));

  // Initialize.
  
  highlight(true);

  widget_update();
  
  // Callbacks.

  _p->widget->callback(widget_callback, this);
}

Text_Edit::~Text_Edit()
{}

void Text_Edit::set(const String & in)
{
  if (in == _value)
    return;
  
  DJV_DEBUG("Text_Edit::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  _value = in;
  
  widget_update();
  
  signal.emit(_value);
}

const String & Text_Edit::get() const
{
  return _value;
}

void Text_Edit::text_font(Fl_Font in)
{
  Text_Base::text_font(in);
  
  _p->widget->textfont(in);
}

void Text_Edit::color(Fl_Color in)
{
  Text_Base::color(in);
  
  _p->widget->color(in);
}

Fl_Color Text_Edit::color() const
{
  return Text_Base::color();
}
  
Fl_Font Text_Edit::text_font() const
{
  return Text_Base::text_font();
}

void Text_Edit::text_color(Fl_Color in)
{
  Text_Base::text_color(in);
  
  _p->widget->textcolor(in);
}

Fl_Color Text_Edit::text_color() const
{
  return Text_Base::text_color();
}

int Text_Edit::handle(int in)
{
  switch (in)
  {
    case FL_KEYBOARD:
    {
      if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
        switch (Fl::event_key())
        {
          case FL_Enter:
          case FL_KP_Enter:
          {
            signal.emit(_p->widget->value());
            action_signal.emit(_p->widget->value());
          }
          return 1;
          
          default: break;
        }
    }
    break;
    
    case FL_UNFOCUS:
      
      signal.emit(_p->widget->value());

      break;
  }
  
  return Text_Base::handle(in);
}

const Fl_Widget * Text_Edit::fltk_widget() const
{
  return _p->widget;
}

void Text_Edit::size_string(const String & in)
{
  _size_string = in;
}
  
const String & Text_Edit::size_string() const
{
  return _size_string;
}

void Text_Edit::dirty()
{
  Text_Base::dirty();

  _p->widget->textfont(text_font());
  _p->widget->textsize(style::global()->font_size());
  _p->widget->textcolor(text_color());
  
  fl_font(text_font(), style::global()->font_size());
  
  size_hint(
    V2i(
      base::string_width(_size_string),
      fl_height()
    ) +
    style::global()->margin_text() * 2 +
    frame_size() * 2
  );
}

void Text_Edit::geom(const Box2i & in)
{
  Text_Base::geom(in);
  
  const Box2i & geom = frame_geom();
  
  _p->widget->resize(geom.x, geom.y, geom.w, geom.h);
}
  
const Box2i & Text_Edit::geom() const
{
  return Text_Base::geom();
}

void Text_Edit::widget_callback()
{
  _value = _p->widget->value();
  
  change_signal.emit(_value);
}

void Text_Edit::widget_update()
{
  _p->widget->value(_value.c_str());
  _p->widget->position(static_cast<int>(_value.size()));
}

//------------------------------------------------------------------------------

}
}

