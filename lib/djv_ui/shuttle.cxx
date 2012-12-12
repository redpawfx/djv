/*------------------------------------------------------------------------------
 lib/djv_ui/shuttle.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "shuttle.h"

#include <djv_ui/icon.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace shuttle {

//------------------------------------------------------------------------------
// Shuttle
//------------------------------------------------------------------------------

Shuttle::Shuttle(const String & prefix) :
  signal(this),
  value_signal(this),
  _value(0.0),
  _pressed(false)
{
  _icon[0] = icon::get(prefix + "shuttle_0");
  _icon[1] = icon::get(prefix + "shuttle_1");
  _icon[2] = icon::get(prefix + "shuttle_2");
  _icon[3] = icon::get(prefix + "shuttle_3");
  _icon[4] = icon::get(prefix + "shuttle_4");
  _icon[5] = icon::get(prefix + "shuttle_5");
  _icon[6] = icon::get(prefix + "shuttle_6");
  _icon[7] = icon::get(prefix + "shuttle_7");
  
  highlight(true);

  size(layout_item::FIXED, layout_item::FIXED);
}

Shuttle::~Shuttle()
{}

int Shuttle::handle(int in)
{
  const V2i mouse(Fl::event_x(), Fl::event_y());
  
  switch (in)
  {
    case FL_PUSH:

      _value = 0.0;
      _pressed = true;
      _mouse_td = mouse;

      base::cursor(FL_CURSOR_WE);
      
      redraw();
      
      signal.emit(true);

      return 1;
      
    case FL_RELEASE:

      _value = 0;
      _pressed = false;

      base::cursor(FL_CURSOR_DEFAULT);
      
      redraw();
      
      signal.emit(false);

      return 1;
    
    case FL_DRAG:
    {
      const int tmp = mouse.x - _mouse_td.x;
      
      if (tmp != _value)
      {
        _value = static_cast<double>(tmp);
        
        redraw();
        
        value_signal.emit(math::round(_value / 5.0));
      }
    }
    return 1;
  }
  
  return Frame::handle(in);
}

void Shuttle::draw()
{
  Widget::draw();

  const Box2i & geom = this->geom();
  
  const bool inside = Widget_Base::inside();
  
  fl_draw_box(
    box_type(style(), _pressed),
    geom.x, geom.y, geom.w, geom.h,
    inside && active() ? style::FL_COLOR_HIGHLIGHT : style::FL_COLOR_BUTTON
  );
  
  const int i = math::mod(math::round(_value / 5.0), 8);
  
  _icon[i]->draw(
    geom.x + (geom.w - _icon[0]->w()) / 2,
    geom.y + (geom.h - _icon[0]->h()) / 2
  );
}

void Shuttle::dirty()
{
  Frame::dirty();
  
  size_hint(V2i(_icon[0]->w(), _icon[0]->h()) + 3 * 2);
}

//------------------------------------------------------------------------------

}
}

