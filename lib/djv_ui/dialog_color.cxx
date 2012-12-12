/*------------------------------------------------------------------------------
 lib/djv_ui/dialog_color.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "dialog.h"

#include <djv_ui/color.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/push_button.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace dialog {

//------------------------------------------------------------------------------
// Color::_Color
//------------------------------------------------------------------------------

struct Color::_Color
{
  Color_Widget * widget;
  Color_Swatch * swatch;
  Push_Button * close;
};

//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------

Color::Color(const djv_image::Color & value) :
  Base(label_color),
  signal(this),
  _p(new _Color)
{
  // Create widgets.

  _p->widget =
    new Color_Widget;
  
  _p->swatch =
    new Color_Swatch;
  _p->swatch->size(layout_item::STRETCH, layout_item::STRETCH);
  
  _p->close =
    new Push_Button(label_close);

  // Layout.

  Layout_V * layout = new Layout_V(this);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout->stretch(layout_h);
  layout_h->add(_p->swatch);
  layout_h->add(_p->widget);
  layout_h->stretch(_p->widget);
  
  layout->add_spacer(-1, true);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.

  size(size_hint());
  
  // Callbacks.

  _p->widget->signal.set(this, widget_callback);
  _p->close->signal.set(this, close_callback);
}

Color::~Color()
{}

void Color::set(const djv_image::Color & in)
{
  if (in == _value)
    return;
  
  _value = in;
  
  widget_update();
  
  signal.emit(_value);
}

const djv_image::Color & Color::get() const
{
  return _value;
}

void Color::widget_callback(const djv_image::Color & in)
{
  set(in);
}

void Color::close_callback(bool)
{
  hide();
}

void Color::widget_update()
{
  callbacks(false);
  
  _p->widget->set(_value);
  _p->swatch->set(_value);
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

