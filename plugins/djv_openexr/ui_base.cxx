/*------------------------------------------------------------------------------
 plugin/djv_openexr/ui_base.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "ui_base.h"

#include <djv_ui/group_box.h>
#include <djv_ui/int_value.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/style.h>

namespace djv_plugin {
namespace openexr {

//------------------------------------------------------------------------------
// djv_ui_image_io()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_ui_image_io()
{
  return new Base_Widget;
}

}

//------------------------------------------------------------------------------
// Base_Widget::_Base_Widget
//------------------------------------------------------------------------------

struct Base_Widget::_Base_Widget
{
  Int_Edit * thread_count;
};

//------------------------------------------------------------------------------
// Base_Widget
//------------------------------------------------------------------------------

Base_Widget::Base_Widget() :
  _plugin(0),
  _p(new _Base_Widget)
{
  DJV_DEBUG("Base_Widget::Base_Widget");

  // Create thread count widgets.
  
  const Base_Option default_value;
  
  group_box::Box_Large * thread_count_group =
    new group_box::Box_Large(label_thread_count_group);
  
  _p->thread_count =
    new Int_Edit;
  _p->thread_count->range(0, 4096);

  Label * thread_count_text =
    new Label(label_thread_count_text);

  // Layout.
  
  Layout_V * layout = 0;
  Layout_V * layout_v = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(thread_count_group);
  thread_count_group->layout()->add(thread_count_text);
  thread_count_group->layout()->add(_p->thread_count);
  
  layout->add_stretch();
  
  // Initialize.

  plugin_update();
  widget_update();
  
  // Callbacks.
  
  _p->thread_count->signal.set(this, thread_count_callback);
}

Base_Widget::~Base_Widget()
{}

plugin::Plugin * Base_Widget::copy() const
{
  return new Base_Widget;
}

String Base_Widget::name() const
{
  return openexr::name;
}

void Base_Widget::set(djv_image::io::Base * in)
{
  _plugin = in;
  
  widget_update();
}

void Base_Widget::thread_count_callback(int in)
{
  _option.thread_count = in;
  
  callback(true);
}

void Base_Widget::callback(bool)
{
  if (! _plugin)
    return;
  
  plugin_update();
  
  signal.emit(_plugin);
}

void Base_Widget::plugin_update()
{
  callbacks(false);

  if (_plugin)
  {
    const List<String> & list = _plugin->option_list();
    
    String tmp;
    tmp << _option.thread_count;
    _plugin->option(list[BASE_THREAD_COUNT], &tmp);
  }

  callbacks(true);
}

void Base_Widget::widget_update()
{
  callbacks(false);

  try
  {
    if (_plugin)
    {
      const List<String> & list = _plugin->option_list();
      
      String tmp;
      tmp = _plugin->option(list[BASE_THREAD_COUNT]);
      tmp >> _option.thread_count;
    }
  }
  catch (String)
  {}
  
  _p->thread_count->set(_option.thread_count);
  
  callbacks(true);
}

//------------------------------------------------------------------------------

}
}

