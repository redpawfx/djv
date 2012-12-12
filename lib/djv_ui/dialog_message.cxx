/*------------------------------------------------------------------------------
 lib/djv_ui/dialog_message.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "dialog.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/push_button.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace dialog {

//------------------------------------------------------------------------------
// Message_Base
//------------------------------------------------------------------------------

Message_Base::Message_Base(const String & title) :
  Base(title)
{}

Message_Base::~Message_Base()
{}

void Message_Base::dirty()
{
  Base::dirty();
  
  _size_min = max(Base::size_min(), V2i(300, 100));
}

const V2i & Message_Base::size_min() const
{
  return _size_min;
}

//------------------------------------------------------------------------------
// Message::_Message
//------------------------------------------------------------------------------

struct Message::_Message
{
  Label * label;
  Push_Button * close;
};

//------------------------------------------------------------------------------
// Message
//------------------------------------------------------------------------------

Message::Message(const String & label) :
  Message_Base(label_message),
  _p(new _Message)
{
  // Create widgets.

  _p->label =
    new Label(label);
  _p->label->align(layout_item::CENTER, layout_item::CENTER);
  
  _p->close =
    new Push_Button(label_close);

  // Layout.

  Layout_V * layout = new Layout_V(this);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout->stretch(layout_h);
  layout_h->margin(style::global()->margin_large());
  layout_h->add(_p->label);
  layout_h->stretch(_p->label);
  
  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.

  size(size_hint());
  
  // Callbacks.

  _p->close->signal.set(this, close_callback);
}

Message::~Message()
{}

void Message::label(const String & in)
{
  _p->label->set(in);
  
  size(size_hint());
}

const String & Message::label() const
{
  return _p->label->get();
}

void Message::show()
{
  _p->close->take_focus();

  Message_Base::show();
}

void Message::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------

}
}

