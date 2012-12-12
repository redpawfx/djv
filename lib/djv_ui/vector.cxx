/*------------------------------------------------------------------------------
 lib/djv_ui/vector.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "vector.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/float_value.h>
#include <djv_ui/int_value.h>

namespace djv_ui {
namespace vector {

//------------------------------------------------------------------------------
// V2i_Edit::_V2i_Edit
//------------------------------------------------------------------------------

struct V2i_Edit::_V2i_Edit
{
  Int_Edit * widget, * widget2;
};

//------------------------------------------------------------------------------
// V2i_Edit
//------------------------------------------------------------------------------

V2i_Edit::V2i_Edit() :
  signal(this),
  _p(new _V2i_Edit)
{
  _p->widget = new Int_Edit;
  _p->widget2 = new Int_Edit;
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  Layout_H * layout = new Layout_H(this);
  layout->margin(0);
  layout->add(_p->widget);
  layout->add(_p->widget2);

  widget_update();
  
  _p->widget->signal.set(this, widget_callback);
  _p->widget2->signal.set(this, widget2_callback);
}

V2i_Edit::~V2i_Edit()
{}

void V2i_Edit::set(const V2i & in)
{
  if (in == _value)
    return;
  
  _value = in;
  
  widget_update();
  
  signal.emit(_value);
}
  
const V2i & V2i_Edit::get() const
{
  return _value;
}

void V2i_Edit::widget_callback(int in)
{
  set(V2i(in, _value.y));
}

void V2i_Edit::widget2_callback(int in)
{
  set(V2i(_value.x, in));
}

void V2i_Edit::widget_update()
{
  callbacks(false);
  
  _p->widget->set(_value.x);
  _p->widget2->set(_value.y);
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// V2f_Edit::_V2f_Edit
//------------------------------------------------------------------------------

struct V2f_Edit::_V2f_Edit
{
  Float_Edit * widget;
  Float_Edit * widget2;
};

//------------------------------------------------------------------------------
// V2f_Edit
//------------------------------------------------------------------------------

V2f_Edit::V2f_Edit() :
  signal(this),
  _p(new _V2f_Edit)
{
  _p->widget = new Float_Edit;
  _p->widget2 = new Float_Edit;
  
  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  Layout_H * layout = new Layout_H(this);
  layout->margin(0);
  layout->add(_p->widget);
  layout->add(_p->widget2);

  widget_update();
  
  _p->widget->signal.set(this, widget_callback);
  _p->widget2->signal.set(this, widget2_callback);
}

V2f_Edit::~V2f_Edit()
{}

void V2f_Edit::set(const V2f & in)
{
  if (in == _value)
    return;
  
  _value = in;
  
  widget_update();
  
  signal.emit(_value);
}

const V2f & V2f_Edit::get() const
{
  return _value;
}

void V2f_Edit::widget_callback(double in)
{
  set(V2f(in, _value.y));
}

void V2f_Edit::widget2_callback(double in)
{
  set(V2f(_value.x, in));
}

void V2f_Edit::widget_update()
{
  callbacks(false);
  
  _p->widget->set(_value.x);
  _p->widget2->set(_value.y);
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

