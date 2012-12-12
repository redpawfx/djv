/*------------------------------------------------------------------------------
 lib/djv_ui/dialog_choice.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "dialog.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/push_button.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace dialog {

//------------------------------------------------------------------------------
// Choice::_Choice
//------------------------------------------------------------------------------

struct Choice::_Choice
{
  Radio_Button_Group * widget;
  
  Label * label;
};

//------------------------------------------------------------------------------
// Choice::Choice
//------------------------------------------------------------------------------

Choice::Choice(const String & label, const List<String> & list) :
  Message_Base(label_choice),
  signal(this),
  _p(new _Choice)
{
  // Create widgets.

  _p->widget =
    new Radio_Button_Group(list);
  _p->widget->align(layout_item::CENTER, layout_item::CENTER);
  
  _p->label =
    new Label(label);

  Push_Button * ok_button =
    new Push_Button(label_ok);
  Push_Button * close_button =
    new Push_Button(label_close);

  // Layout.

  Layout_H * layout_h = 0;
  Layout_V * layout_v = 0;
  
  Layout_V * layout = new Layout_V(this);
  
  layout_v = new Layout_V(layout);
  layout->stretch(layout_v);
  layout_v->margin(style::global()->margin_large());
  layout_v->add(_p->label);
  layout_v->add(_p->widget);
  layout_v->stretch(_p->widget);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(ok_button);
  layout_h->add(close_button);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.

  size(size_hint(), false);

  // Callbacks.
  
  ok_button->signal.set(this, ok_callback);
  close_button->signal.set(this, close_callback);
}

Choice::~Choice()
{}

void Choice::list(const List<String> & in)
{
  //_p->widget->list(in);
  _p->widget->label(in);
  
  size(size_hint(), false);
}

List<String> Choice::list() const
{
  //return _p->widget->list();
  return _p->widget->label();
}

void Choice::label(const String & in)
{
  _p->label->set(in);
  
  size(size_hint(), false);
}

const String & Choice::label() const
{
  //return _p->label->get();
  return _p->widget->title();
}

void Choice::show()
{
  _p->widget->take_focus();

  Message_Base::show();
}

void Choice::ok_callback(bool)
{
  signal.emit(_p->widget->get());
  
  hide();
}

void Choice::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------

}
}

