/*------------------------------------------------------------------------------
 lib/djv_ui/radio_button_group.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "radio_button.h"

#include <djv_ui/group_box.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace radio_button {

//------------------------------------------------------------------------------
// Radio_Button_Group::_Radio_Button_Group
//------------------------------------------------------------------------------

struct Radio_Button_Group::_Radio_Button_Group
{
  group_box::Box_Small * widget_group;
  Layout_V * widget_layout;
  
  Layout_V * layout;
  
  List<Radio_Button *> widgets;
};

//------------------------------------------------------------------------------
// Radio_Button_Group
//------------------------------------------------------------------------------

void Radio_Button_Group::init()
{
  debug_path("Radio_Button_Group");
  
  _p->widget_group = new group_box::Box_Small;
  
  _p->layout = new Layout_V(this);
  _p->layout->margin(0);
  _p->layout->add(_p->widget_group);
  _p->widget_layout = new Layout_V(_p->layout);
  _p->widget_layout->margin(0);
  
  widget_update();
  value_update();
}

Radio_Button_Group::Radio_Button_Group(
  const List<String> & label
) :
  signal(this),
  _value(-1),
  _label(label),
  _p(new _Radio_Button_Group)
{
  init();
}

Radio_Button_Group::Radio_Button_Group(
  const String & title,
  const List<String> & label
) :
  signal(this),
  _value(-1),
  _title(title),
  _label(label),
  _p(new _Radio_Button_Group)
{
  init();
}

Radio_Button_Group::~Radio_Button_Group()
{}

void Radio_Button_Group::set(int in)
{
  DJV_DEBUG("Radio_Button_Group::set");
  
  const int size = static_cast<int>(_p->widgets.size());

  in = math::clamp(in, 0, size - 1);

  DJV_DEBUG_PRINT("in = " << in);
  
  if (in == _value)
    return;
  
  _value = in;
  
  value_update();

  signal.emit(_value);
}

int Radio_Button_Group::get() const
{
  return _value;
}

void Radio_Button_Group::title(const String & in)
{
  if (in == _title)
    return;
  
  _title = in;
  
  widget_update();
}
  
const String & Radio_Button_Group::title() const
{
  return _title;
}

void Radio_Button_Group::label(const List<String> & in)
{
  if (in == _label)
    return;
  
  _label = in;
  
  widget_update();
}
  
const List<String> & Radio_Button_Group::label() const
{
  return _label;
}

void Radio_Button_Group::widget_callback(int in)
{
  DJV_DEBUG("Radio_Button_Group::widget_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  set(in);
}

void Radio_Button_Group::value_update()
{
  DJV_DEBUG("Radio_Button_Group::value_update");
  
  callbacks(false);
  
  if (_value >= 0 && _value < _p->widgets.size())
    _p->widgets[_value]->set(true);

  callbacks(true);
}

void Radio_Button_Group::widget_update()
{
  DJV_DEBUG("Radio_Button_Group::widget_update");
  
  callbacks(false);
  
  // Create widgets.
  
  _p->widget_group->layout()->del();
  _p->widget_layout->del();
  
  _p->widgets.resize(_label.size());
  
  for (list::size_type i = 0; i < _p->widgets.size(); ++i)
    _p->widgets[i] = new Radio_Button(_label[i]);
  
  radio_button::group(_p->widgets);
  
  // Layout.
  
  layout::Layout * layout = 0;
  
  if (! _title.empty())
  {
    layout = _p->widget_group->layout();
  }
  else
  {
    layout = _p->widget_layout;
  }
  
  for (list::size_type i = 0; i < _p->widgets.size(); ++i)
    layout->add(_p->widgets[i]);
  
  // Initialize.
  
  if (! _title.empty())
  {
    _p->widget_group->label(_title);
    _p->widget_group->show();
  }
  else
  {
    _p->widget_group->hide();
  }
  
  update();
  
  // Callbacks.
  
  for (list::size_type i = 0; i < _p->widgets.size(); ++i)
    _p->widgets[i]->group_signal.set(this, widget_callback);

  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

