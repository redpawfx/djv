/*------------------------------------------------------------------------------
 lib/djv_ui/text.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "text.h"

#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace text {

//------------------------------------------------------------------------------
// Text_Base
//------------------------------------------------------------------------------

Text_Base::Text_Base() :
  _draw_select(false)
{  
  style(frame::STYLE_BORDER);
  
  size(layout_item::STRETCH, layout_item::FIXED);
}

Text_Base::~Text_Base()
{}

int Text_Base::handle(int in)
{
  switch (in)
  {
    case FL_KEYBOARD:
    {
      if (! Fl::event_state(FL_SHIFT | FL_CTRL | FL_ALT))
        switch (Fl::event_key())
        {
          case FL_Escape:
          {
            Fl::focus(0);
          }
          return 1;
          
          default: break;
        }
    }
    break;
  }
  
  return Frame::handle(in);
}

void Text_Base::draw()
{
  const bool inside = Widget_Base::inside();
  
  color(
    _draw_select && active() ?
      FL_SELECTION_COLOR :
      (inside && active() ? style::FL_COLOR_HIGHLIGHT : FL_BACKGROUND_COLOR)
  );
  
  Frame::draw();
}

void Text_Base::draw_select(bool in)
{
  if (in == _draw_select)
    return;
  
  _draw_select = in;
  
  redraw();
}

void Text_Base::dirty()
{
  Frame::dirty();
}

//------------------------------------------------------------------------------

}
}

