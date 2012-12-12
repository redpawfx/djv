/*------------------------------------------------------------------------------
 lib/djv_ui/image_pixel.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "image_pixel.h"

#include <djv_ui/choice.h>
#include <djv_ui/layout_row.h>

namespace djv_ui {
namespace image {

//------------------------------------------------------------------------------
// Pixel::_Pixel
//------------------------------------------------------------------------------

struct Pixel::_Pixel
{
  Choice * widget;
};

//------------------------------------------------------------------------------
// Pixel
//------------------------------------------------------------------------------

Pixel::Pixel() :
  signal(this),
  _value(djv_image::PIXEL(0)),
  _p(new _Pixel)
{
  debug_path("image::Pixel");
  
  add(_p->widget = new Choice(djv_image::pixel::label_pixel()));
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  widget_update();
  
  _p->widget->signal.set(this, widget_callback);
}

Pixel::~Pixel()
{}

void Pixel::del()
{
  widget::Widget::del();
  
  _p->widget->del();
}

void Pixel::set(djv_image::PIXEL in)
{
  if (in == _value)
    return;
  
  _value = in;
  
  widget_update();
  
  signal.emit(_value);
}

djv_image::PIXEL Pixel::get() const
{
  return _value;
}

void Pixel::draw()
{
  widget::Widget::draw();
}

void Pixel::dirty()
{
  widget::Widget::dirty();
  
  _p->widget->dirty();
  
  size_hint(_p->widget->size_hint());
}

void Pixel::geom(const Box2i & in)
{
  widget::Widget::geom(in);
  
  _p->widget->geom(in);
}

const Box2i & Pixel::geom() const
{
  return widget::Widget::geom();
}

void Pixel::widget_callback(int in)
{
  set(static_cast<djv_image::PIXEL>(in));
}

void Pixel::widget_update()
{
  callbacks(false);
  
  _p->widget->set(_value);
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

