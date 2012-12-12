/*------------------------------------------------------------------------------
 lib/djv_ui/label_image.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "label_private.h"

#include <djv_ui/style.h>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace label {

//------------------------------------------------------------------------------
// Image_Label
//------------------------------------------------------------------------------

Image_Label::Image_Label(Fl_Image * in) :
   push_signal(this),
  _value(in)
{
  debug_path("Image_Label");
}

Image_Label::~Image_Label()
{}

void Image_Label::set(Fl_Image * in)
{
  _value = in;

  update();
  redraw();
}

Fl_Image * Image_Label::get() const
{
  return _value;
}

int Image_Label::handle(int in)
{
  switch (in)
  {
    case FL_PUSH:
      push_signal.emit(true);
      return 1;
  }
  
  return Widget::handle(in);
}

void Image_Label::draw()
{
  Widget::draw();
  
  const Box2i & geom = this->geom();
  
  if (_value)
    _value->draw(
      geom.x + (geom.w - _value->w()) / 2,
      geom.y + (geom.h - _value->h()) / 2
    );
}

void Image_Label::dirty()
{
  Widget::dirty();
  
  size_hint(_value ? V2i(_value->w(), _value->h()) : V2i());
}

Layout_Item * Image_Label::group_align()
{
  return 0;
}

Layout_Item * Image_Label::label_align()
{
  return this;
}

//------------------------------------------------------------------------------

}
}

