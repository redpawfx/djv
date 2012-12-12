/*------------------------------------------------------------------------------
 lib/djv_ui/scroll.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "scroll.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>

namespace djv_ui {
namespace scroll {

//------------------------------------------------------------------------------
// Scroll
//------------------------------------------------------------------------------

Scroll::Scroll() :
  Fl_Scroll(0, 0, 0, 0),
  _resizable(true)
{
  debug_path("Scroll");
  
  Fl_Scroll::end();

  //Fl_Scroll::type(Fl_Scroll::BOTH_ALWAYS);

  Fl_Scroll::box(style::FL_BOX_BORDER);
  Fl_Scroll::scrollbar.slider(style::FL_BOX_THIN_UP);
  Fl_Scroll::hscrollbar.slider(style::FL_BOX_THIN_UP);
  
  add(_dummy_widget = new Fl_Box(0, 0, 0, 0));
  _dummy_widget->box(FL_NO_BOX);
  _dummy_widget->hide();
  
  //Layout_V * layout = new Layout_V(this);
  //layout->margin(0);
  new Layout_V(this);
}

Scroll::~Scroll()
{}

int Scroll::handle(int in)
{
  switch (Widget_Base::handle(in))
  {
    case -1: return 0;
    case 1: return 1;
  }
  
  return Fl_Scroll::handle(in);
}

void Scroll::show()
{
  Fl_Scroll::show();
  
  Widget_Base::show();
}

void Scroll::draw()
{
  Fl_Scroll::draw();
  
  Widget_Base::draw();
}

void Scroll::resizable(bool in)
{
  _resizable = in;
}

bool Scroll::resizable() const
{
  return _resizable;
}

void Scroll::dirty()
{
  DJV_DEBUG("Scroll::dirty");
  
  Widget_Base::dirty();

  size_hint(
    V2i(
      Fl_Scroll::scrollbar.w(),
      Fl_Scroll::hscrollbar.h()
    ) +
    1 * 2
  );

  _size_min = style::global()->size_text();
}

void Scroll::geom(const Box2i & in)
{
  DJV_DEBUG("Scroll::geom");
  DJV_DEBUG_PRINT("in = " << in);

  Fl_Scroll::resize(in.x, in.y, in.w, in.h);
  
  Layout_Item::geom(in);

  const V2i & size_hint = layout()->size_hint();
  
  DJV_DEBUG_PRINT("size hint = " << size_hint);

  const V2i scroll(Fl_Scroll::xposition(), Fl_Scroll::yposition());
  
  DJV_DEBUG_PRINT("scroll = " << scroll);
  
  V2i size;
  if (_resizable)
  {
    size = in.size + scroll;
    
    // XXX Scrollbar visibility.

    //size -= V2i(Fl_Scroll::scrollbar.w(), Fl_Scroll::hscrollbar.h());
    
    if (scroll.y || size_hint.y > size.y)
      size.x -= Fl_Scroll::scrollbar.w();
    if (scroll.x || size_hint.x > size.x)
      size.y -= Fl_Scroll::hscrollbar.h();
  }
  else
  {
    size = size_hint;
  }

  Box2i geom(in.position - scroll, size);
  
  // XXX Frame border.
  
  geom = box::border(geom, V2i(-1));

  _dummy_widget->resize(geom.x, geom.y, geom.w, geom.h);
  
  layout()->geom(geom);
}

const Box2i & Scroll::geom() const
{
  return Widget_Base::geom();
}

void Scroll::resize(int x, int y, int w, int h)
{
  geom(Box2i(x, y, w, h));
}

const V2i & Scroll::size_min() const
{
  return _size_min;
}

int Scroll::visible() const
{
  return Fl_Scroll::visible();
}

//------------------------------------------------------------------------------

}
}

