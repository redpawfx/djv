/*------------------------------------------------------------------------------
 lib/djv_ui/check_button_group.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "check_button.h"

#include <djv_ui/group_box.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/label.h>
#include <djv_ui/style.h>

namespace djv_ui {
namespace check_button {

//------------------------------------------------------------------------------
// Check_Button_Group::_Check_Button_Group
//------------------------------------------------------------------------------

struct Check_Button_Group::_Check_Button_Group
{
  group_box::Box_Small * widget_group;
  Layout_V * widget_layout;
  
  Layout_V * layout;
  
  List<Check_Button *> widgets;
};

//------------------------------------------------------------------------------
// Check_Button_Group
//------------------------------------------------------------------------------

void Check_Button_Group::init()
{
  debug_path("Check_Button_Group");
  
  _p->widget_group = new group_box::Box_Small;
  
  _p->layout = new Layout_V(this);
  _p->layout->margin(0);
  _p->layout->add(_p->widget_group);
  _p->widget_layout = new Layout_V(_p->layout);
  _p->widget_layout->margin(0);
  
  widget_update();
  value_update();
}

Check_Button_Group::Check_Button_Group(
  const List<String> & label
) :
  signal(this),
  index_signal(this),
  _label(label),
  _p(new _Check_Button_Group)
{
  init();
}

Check_Button_Group::Check_Button_Group(
  const String & title,
  const List<String> & label
) :
  signal(this),
  index_signal(this),
  _title(title),
  _label(label),
  _p(new _Check_Button_Group)
{
  init();
}

Check_Button_Group::~Check_Button_Group()
{}

void Check_Button_Group::set(const List<bool> & in)
{
  if (in == _value)
    return;

  _value = in;
  
  value_update();

  signal.emit(_value);
}

void Check_Button_Group::set_index(const List<int> & in)
{
  const List<int> & index = get_index();
  
  if (in == index)
    return;

  for (list::size_type i = 0; i < _value.size(); ++i)
    _value[i] = list::find(static_cast<int>(i), in) != -1;
  
  value_update();

  signal.emit(_value);
}

const List<bool> & Check_Button_Group::get() const
{
  return _value;
}

const List<int> Check_Button_Group::get_index() const
{
  List<int> out;
  
  for (list::size_type i = 0; i < _value.size(); ++i)
    if (_value[i])
      out += static_cast<int>(i);
  
  return out;
}

void Check_Button_Group::title(const String & in)
{
  if (in == _title)
    return;
  
  _title = in;
  
  widget_update();
}
  
const String & Check_Button_Group::title() const
{
  return _title;
}

void Check_Button_Group::label(const List<String> & in)
{
  if (in == _label)
    return;
  
  _label = in;
  
  widget_update();
}
  
const List<String> & Check_Button_Group::label() const
{
  return _label;
}

void Check_Button_Group::widget_callback(bool)
{
  List<bool> value;
  for (list::size_type i = 0; i < _p->widgets.size(); ++i)
    value += _p->widgets[i]->get();
  
  set(value);
}

void Check_Button_Group::value_update()
{ 
  callbacks(false);
  
  for (
    list::size_type i = 0;
    i < _value.size() && i < _p->widgets.size();
    ++i
  )
    _p->widgets[i]->set(_value[i]);
  
  callbacks(true);
}

void Check_Button_Group::widget_update()
{
  callbacks(false);
  
  // Create widgets.
  
  _p->widget_group->layout()->del();
  _p->widget_layout->del();
  
  _p->widgets.resize(_label.size());
  
  for (list::size_type i = 0; i < _p->widgets.size(); ++i)
    _p->widgets[i] = new Check_Button(_label[i]);
  
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
    _p->widgets[i]->signal.set(this, widget_callback);

  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

