/*------------------------------------------------------------------------------
 lib/djv_ui/dialog_input.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "dialog.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/push_button.h>
#include <djv_ui/style.h>
#include <djv_ui/text.h>

namespace djv_ui {
namespace dialog {

//------------------------------------------------------------------------------
// Input::_Input
//------------------------------------------------------------------------------

struct Input::_Input
{
  Text_Edit * widget;
  Label * label;
};

//------------------------------------------------------------------------------
// Input
//------------------------------------------------------------------------------

Input::Input(const String & label, const String & value) :
  Message_Base(label_input),
  signal(this),
  _p(new _Input)
{
  // Create widgets.

  _p->widget = new Text_Edit(value);
  _p->label = new Label(label);

  Push_Button * ok_button = new Push_Button(label_ok);
  Push_Button * close_button = new Push_Button(label_close);

  // Layout.

  Layout_V * layout = new Layout_V(this);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout->stretch(layout_h);
  layout_h->margin(style::global()->margin_large());
  layout_h->add(_p->label);
  layout_h->add(_p->widget);
  layout_h->stretch(_p->widget);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(ok_button);
  layout_h->add(close_button);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.

  size(size_hint());

  // Callbacks.
  
  _p->widget->action_signal.set(this, widget_callback);
  ok_button->signal.set(this, ok_callback);
  close_button->signal.set(this, close_callback);
}

Input::~Input()
{}

void Input::set(const String & in)
{
  _p->widget->set(in);
  
  size(size_hint());
}

String Input::get() const
{
  return _p->widget->get();
}

void Input::label(const String & in)
{
  _p->label->set(in);
  
  size(size_hint());
}

const String & Input::label() const
{
  return _p->label->get();
}

void Input::show()
{
  _p->widget->take_focus();

  Message_Base::show();
}

void Input::widget_callback(const String &)
{
  ok_callback(true);
}

void Input::ok_callback(bool)
{
  signal.emit(_p->widget->get());
  
  hide();
}

void Input::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------

}
}

