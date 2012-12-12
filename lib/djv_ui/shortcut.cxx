/*------------------------------------------------------------------------------
 lib/djv_ui/shortcut.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "shortcut.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/scroll.h>
#include <djv_ui/text.h>
#include <djv_ui/tool_button.h>
#include <FL/Fl.H>

namespace djv_ui {
namespace shortcut {

//------------------------------------------------------------------------------
// _Callback_Data
//------------------------------------------------------------------------------

namespace {

struct _Callback_Data
{
  _Callback_Data(widget::Widget * widget, int value) :
    widget(widget),
    value(value)
  {}

  widget::Widget * widget;
  int value;
};

}

//------------------------------------------------------------------------------
// _Edit
//------------------------------------------------------------------------------

namespace {

class _Edit : public Text_Edit
{
public:

  _Edit(int = 0);
  
  void set(int);
  
  int get() const { return _value; }
  
  callback::Signal<const void *> signal;
  
private:

  DJV_CALLBACK(_Edit, widget_callback, const String &);

  void widget_update();
  
  int _value;
};

_Edit::_Edit(int value) :
  signal(this),
  _value(value)
{
  // Initialize.
  
  style(frame::STYLE_EMPTY);
  
  widget_update();
  
  // Callbacks.
  
  Text_Edit::signal.set(this, widget_callback);
}

void _Edit::set(int value)
{
  if (value == _value) return;
  
  DJV_DEBUG("_Edit::set");
  DJV_DEBUG_PRINT("value = " << value);
  
  _value = value;
  
  widget_update();
  
  _Callback_Data data(this, _value);
  signal.emit(&data);
}

bool alt(int in)
{
  return 
    in == FL_Shift_L ||
    in == FL_Shift_R ||
    in == FL_Alt_L ||
    in == FL_Alt_R ||
    in == FL_Control_L ||
    in == FL_Control_R ||
    in == FL_Meta_L ||
    in == FL_Meta_R;
}

void _Edit::widget_callback(const String & in)
{
  int value = 0;
  String tmp = in;
  shortcut::serialize(&tmp, &value);
  set(value);

  widget_update();
}

void _Edit::widget_update()
{
  callbacks(false);
  
  Text_Edit::set(shortcut::serialize(_value));

  callbacks(true);
}

}

//------------------------------------------------------------------------------
// _Button
//------------------------------------------------------------------------------

namespace {

class _Button : public Tool_Button
{
public:

  _Button();
  
  callback::Signal<const void *> signal;
  
  int handle(int);
  
private:

  DJV_CALLBACK(_Button, widget_callback, bool);
};

_Button::_Button() :
  Tool_Button("tool_add"),
  signal(this)
{
  type(tool_button::TOGGLE);
  tooltip(tooltip_button);
  
  Tool_Button::signal.set(this, widget_callback);
}

void _Button::widget_callback(bool in)
{
  take_focus();
}

int _Button::handle(int in)
{
  switch (in)
  {
    case FL_KEYBOARD:
    {
      int key = Fl::event_key();
      if (get() && ! alt(key))
      {
        if (Fl::event_shift()) key |= FL_SHIFT;
        if (Fl::event_alt()) key |= FL_ALT;
        if (Fl::event_ctrl()) key |= FL_CTRL;
        
        set(false);
        
        _Callback_Data data(this, key);
        signal.emit(&data);
        
        return 1;
      }
    }
    break;
    
    case FL_FOCUS:
      if (get()) return 1;
      break;

    case FL_UNFOCUS:
      set(false);
      return 1;
  }
  
  return Tool_Button::handle(in);
}

}

//------------------------------------------------------------------------------
// Shortcut_Widget::_Shortcut_Widget
//------------------------------------------------------------------------------

struct Shortcut_Widget::_Shortcut_Widget
{
  scroll::Scroll * scroll;
  
  Layout_V * layout;
  
  List<Label *> label;
  List<_Edit *> edit;
  List<_Button *> button;
};

//------------------------------------------------------------------------------
// Shortcut_Widget
//------------------------------------------------------------------------------

Shortcut_Widget::Shortcut_Widget() :
  signal(this),
  _p(new _Shortcut_Widget)
{
  _p->scroll = new scroll::Scroll;

  Layout_V * layout = new Layout_V(this);
  layout->margin(0);
  layout->add(_p->scroll);
  
  _p->layout = new Layout_V(_p->scroll);
  _p->layout->spacing(1);
  
  widget_update();
  value_update();
}

Shortcut_Widget::~Shortcut_Widget()
{}

void Shortcut_Widget::set(const List<Shortcut> & in)
{
  if (in == _list) 
    return;
  
  _list = in;
  
  widget_update();
  value_update();
  
  signal.emit(_list);
}

const List<Shortcut> & Shortcut_Widget::get() const
{
  return _list;
}

void Shortcut_Widget::widget_callback(const void * in)
{
  const _Callback_Data * data = static_cast<const _Callback_Data *>(in);
  
  for (list::size_type i = 0; i < _list.size(); ++i)
    if (data->widget == _p->edit[i] || data->widget == _p->button[i])
    {
      _list[i].value = data->value;
      
      _p->edit[i]->set(_list[i].value);
      
      break;
    }

  value_update();

  signal.emit(_list);
}

void Shortcut_Widget::widget_update()
{
  DJV_DEBUG("Shortcut_Widget::widget_update");
  
  callbacks(false);
  
  _p->layout->del();
  _p->edit.clear();
  _p->button.clear();
  
  List<Layout_Item *> layout_group;
  for (list::size_type i = 0; i < _list.size(); ++i)
  {
    Label * label = new Label(_list[i].name);
    _Edit * edit = new _Edit(_list[i].value);
    _Button * button = new _Button;

    layout_group += label;
    Layout_H * layout = new Layout_H(_p->layout);
    layout->margin(0);
    layout->add(label);
    layout->add(edit);
    layout->add(button);

    edit->signal.set(this, widget_callback);
    button->signal.set(this, widget_callback);

    _p->label += label;
    _p->edit += edit;
    _p->button += button;
  }
  _p->layout->add_stretch();
  layout_item::group(layout_group);
  
  callbacks(true);
}

void Shortcut_Widget::value_update()
{
  callbacks(false);
  
  for (list::size_type i = 0; i < _list.size(); ++i)
  {
    _p->edit[i]->color(FL_BACKGROUND2_COLOR);
    _p->edit[i]->text_color(text_color());
  }
  
  for (list::size_type i = 0; i < _list.size(); ++i)
    for (list::size_type j = 0; j < _list.size(); ++j)
      if (
        i != j &&
        _list[i].value &&
        _list[i].value == _list[j].value
      ) {
        _p->edit[i]->color(FL_SELECTION_COLOR);
        _p->edit[i]->text_color(FL_BLACK);
        break;
      }
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

