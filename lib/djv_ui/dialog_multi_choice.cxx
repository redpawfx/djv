/*------------------------------------------------------------------------------
 lib/djv_ui/dialog_multi_choice.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "dialog.h"

#include <djv_ui/check_button.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/push_button.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace dialog {

//------------------------------------------------------------------------------
// Multi_Choice::_Multi_Choice
//------------------------------------------------------------------------------

struct Multi_Choice::_Multi_Choice
{
  Check_Button_Group * widget;
  Label * label;
};

//------------------------------------------------------------------------------
// Multi_Choice
//------------------------------------------------------------------------------

Multi_Choice::Multi_Choice(const String & label, const List<String> & list) :
  Message_Base(label_multi_choice),
  signal(this),
  index_signal(this),
  _p(new _Multi_Choice)
{
  // Create widgets.

  _p->widget =
    new Check_Button_Group(list);
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

Multi_Choice::~Multi_Choice()
{}

void Multi_Choice::list(const List<String> & in)
{
  _p->widget->label(in);
  
  size(size_hint(), false);
}

List<String> Multi_Choice::list() const
{
  return _p->widget->label();
}

void Multi_Choice::label(const String & in)
{
  _p->label->set(in);
  
  size(size_hint(), false);
}

const String & Multi_Choice::label() const
{
  return _p->widget->title();
}

void Multi_Choice::show()
{
  _p->widget->set(List<bool>());
  
  _p->widget->take_focus();

  Message_Base::show();
}

void Multi_Choice::ok_callback(bool)
{
  signal.emit(_p->widget->get());
  index_signal.emit(_p->widget->get_index());
  
  hide();
}

void Multi_Choice::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------

}
}

