/*------------------------------------------------------------------------------
 lib/djv_ui/dialog_question.cxx
 
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
// Question::_Question
//------------------------------------------------------------------------------

struct Question::_Question
{
  Label * label;
  Push_Button * ok, * close;
};

//------------------------------------------------------------------------------
// Question
//------------------------------------------------------------------------------

Question::Question(const String & label) :
  Message_Base(label_question),
  signal(this),
  _p(new _Question)
{
  // Create widgets.

  _p->label =
    new Label(label);
  _p->label->align(layout_item::CENTER, layout_item::CENTER);

  _p->ok =
    new Push_Button(label_yes);
  _p->close =
    new Push_Button(label_no);

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
  layout_h->add(_p->ok);
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.

  size(size_hint(), false);

  // Callbacks.
  
  _p->ok->signal.set(this, ok_callback);
  _p->close->signal.set(this, close_callback);
}

Question::~Question()
{}

void Question::label(const String & in)
{
  _p->label->set(in);
  
  size(size_hint(), false);
}

const String & Question::label() const
{
  return _p->label->get();
}

void Question::show()
{
  _p->close->take_focus();

  Message_Base::show();
}

void Question::ok_callback(bool)
{
  signal.emit(true);
  
  hide();
}

void Question::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------

}
}
