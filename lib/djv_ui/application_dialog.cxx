/*------------------------------------------------------------------------------
 lib/djv_ui/application_dialog.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "application_dialog.h"

#include <djv_ui/application.h>
#include <djv_ui/check_button.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/prefs.h>
#include <djv_ui/push_button.h>
#include <djv_ui/scroll.h>
#include <djv_ui/style.h>
#include <djv_ui/text.h>
#include <FL/Fl.H>
#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>
#include "durant.xpm"

namespace djv_ui {
namespace application_dialog {

//------------------------------------------------------------------------------
// Message_Dialog::_Message_Dialog
//------------------------------------------------------------------------------

struct Message_Dialog::_Message_Dialog
{
  Text_Multiline_Display * widget;
  Check_Button * show;
  Push_Button * clear, * close;
};

//------------------------------------------------------------------------------
// Message_Dialog
//------------------------------------------------------------------------------

Message_Dialog::Message_Dialog() :
  dialog::Base(label_message),
  _show(true),
  _p(new _Message_Dialog)
{
  // Create widgets.
  
  _p->widget =
    new Text_Multiline_Display;
  
  _p->show =
    new Check_Button(dialog::label_show);
  
  _p->clear =
    new Push_Button(label_message_clear);
  
  _p->close =
    new Push_Button(dialog::label_close);
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->add(_p->widget);
  layout->stretch(_p->widget);

  Layout_H * layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add(_p->show);
  layout_h->add_stretch();
  layout_h->add(_p->clear);
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Preferences.
  
  prefs::Prefs prefs(prefs::prefs_global(), "message_dialog");
  prefs::get(&prefs, "show", &_show);

  // Initialize.
  
  widget_update();
  dialog_update();
  
  size(size_hint());

  // Callbacks.

  _p->show->signal.set(this, show_callback);
  _p->clear->signal.set(this, clear_callback);
  _p->close->signal.set(this, close_callback);
}

Message_Dialog::~Message_Dialog()
{}

void Message_Dialog::del()
{
  prefs::Prefs prefs(prefs::prefs_global(), "message_dialog");
  prefs::set(&prefs, "show", _show);

  dialog::Base::del();
}

void Message_Dialog::set(const String & in)
{
  _list.push_front(in);
  if (_list.size() > 100)
    _list.pop_back();
  
  widget_update();
  
  if (_show)
    show();
}

void Message_Dialog::set(const Error & in)
{
  set(djv_base::error::format(in));
}

void Message_Dialog::show()
{
  _p->close->take_focus();

  dialog::Base::show();
}

void Message_Dialog::dirty()
{
  Base::dirty();
  
  _size_min = max(Base::size_min(), V2i(400, 200));
}

const V2i & Message_Dialog::size_min() const
{
  return _size_min;
}

void Message_Dialog::show_callback(bool in)
{
  _show = in;
  
  dialog_update();
}

void Message_Dialog::clear_callback(bool)
{
  _list.clear();
  
  widget_update();
}

void Message_Dialog::close_callback(bool)
{
  hide();
}

void Message_Dialog::widget_update()
{
  callbacks(false);
  
  _p->widget->set(string::join(_list, "\n"));
  
  callbacks(true);
}

void Message_Dialog::dialog_update()
{
  callbacks(false);
  
  _p->show->set(_show);
  
  callbacks(true);
}

//------------------------------------------------------------------------------
// Info_Dialog::_Info_Dialog
//------------------------------------------------------------------------------

struct Info_Dialog::_Info_Dialog
{
  Text_Multiline_Display * widget;
  
  Push_Button * close;
};

//------------------------------------------------------------------------------
// Info_Dialog
//------------------------------------------------------------------------------

Info_Dialog::Info_Dialog() :
  dialog::Base(label_info),
  _p(new _Info_Dialog)
{
  DJV_DEBUG("Info_Dialog::Info_Dialog");

  // Create widgets.

  _p->widget = new Text_Multiline_Display(application::global()->info());
  _p->widget->word_wrap(true);
  
  _p->close = new Push_Button(dialog::label_close);
    
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  
  layout->add(_p->widget);
  layout->stretch(_p->widget);
  
  Layout_H * layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.
  
  size(V2i(500, 500));

  // Callbacks.

  _p->close->signal.set(this, close_callback);
}

Info_Dialog::~Info_Dialog()
{}

void Info_Dialog::show()
{
  dialog::Base::show();

  _p->close->take_focus();
}

void Info_Dialog::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------
// About_Dialog::_About_Dialog
//------------------------------------------------------------------------------

struct About_Dialog::_About_Dialog
{
  std::auto_ptr<Fl_Image> image;
  
  Push_Button * close;
};

//------------------------------------------------------------------------------
// About_Dialog
//------------------------------------------------------------------------------

About_Dialog::About_Dialog() :
  dialog::Base(label_about),
  _p(new _About_Dialog)
{
  _p->image = std::auto_ptr<Fl_Image>(new Fl_Pixmap((const char **)durant_xpm));

  // Create widgets.
  
  Image_Label * image = new Image_Label(_p->image.get());

  scroll::Scroll * scroll = new scroll::Scroll;
  Multiline_Label * label = new Multiline_Label(application::global()->about());
  //Label * label = new Label(application::global()->about());
  label->align(layout_item::LEFT, layout_item::LEFT);
  
  _p->close = new Push_Button(dialog::label_close);
    
  // Layout.
  
  Layout_V * layout = 0;
  Layout_H * layout_h = 0;
  Layout_V * layout_scroll = 0;
  
  layout = new Layout_V(this);
  layout->add(image);
  layout->add(scroll);
  layout->stretch(scroll);
  layout_scroll = new Layout_V(scroll);
  layout_scroll->add(label);
  layout_scroll->stretch(label);

  layout_h = new Layout_H(layout);
  layout_h->margin(0);
  layout_h->add_stretch();
  layout_h->add(_p->close);
  layout_h->add_spacer(layout::window_handle_size());
  
  // Initialize.
  
  size(V2i(500, 500));

  // Callbacks.

  _p->close->signal.set(this, close_callback);
}

About_Dialog::~About_Dialog()
{}

void About_Dialog::show()
{
  _p->close->take_focus();

  dialog::Base::show();
}

void About_Dialog::close_callback(bool)
{
  hide();
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  label_message = "Messages",
  label_message_clear = "&Clear",
  label_info = "Information",
  label_about = "About";

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  _message(new Message_Dialog),
  _info(new Info_Dialog),
  _about(new About_Dialog)
{}

Global::~Global()
{}

Message_Dialog * Global::message() const
{
  return _message.get();
}

Info_Dialog * Global::info() const
{
  return _info.get();
}

About_Dialog * Global::about() const
{
  return _about.get();
}

void message(const String & in)
{
  global()->message()->set(in);
}

void error(const Error & in)
{
  global()->message()->set(in);
}

//------------------------------------------------------------------------------

}
}

