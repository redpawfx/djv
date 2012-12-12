/*------------------------------------------------------------------------------
 lib/djv_ui/global.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "global.h"

#include <djv_ui/application.h>
#include <djv_ui/window.h>
#include <algorithm>

namespace djv_ui {

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  window_add_signal(this),
  window_del_signal(this),
  window_focus_signal(this)
{
  DJV_DEBUG("Global::Global");
}

Global::~Global()
{
  DJV_DEBUG("Global::~Global");
}

void Global::widget_add(Widget_Base * in)
{
  DJV_DEBUG("Global::widget_add");
  DJV_DEBUG_PRINT("in = " << long(in));
  
  _widget_list += in;
}

void Global::widget_del(Widget_Base * in)
{
  //DJV_DEBUG("Global::widget_del");
  
  List<Widget_Base *>::iterator i = std::find(
    _widget_list.begin(), _widget_list.end(), in
  );
  if (i != _widget_list.end())
    _widget_list.erase(i);
}

const Widget_Base * Global::widget_get(const Fl_Widget * in)
{
  const Widget_Base * out = 0;
  
  for (list::size_type i = 0; i < _widget_list.size(); ++i)
    if (in == _widget_list[i]->fltk_widget())
    {
      out = _widget_list[i];
      break;
    }
  
  return out;
}

void Global::window_add(Window * in)
{
  //DJV_DEBUG("Global::window_add");
  
  _window_list += in;
  
  window_add_signal.emit(in);
}

void Global::window_del(Window * in)
{
  //DJV_DEBUG("Global::window_del");
  
  List<Window *>::iterator i = std::find(
    _window_list.begin(), _window_list.end(), in
  );
  
  if (i != _window_list.end())
  {
    _window_list.erase(i);

    window_del_signal.emit(in);
  }
}

void Global::window_focus(Window * in)
{
  //DJV_DEBUG("Global::window_focus");
  
  window_focus_signal.emit(in);
}

void Global::window_redraw()
{
  DJV_DEBUG("Global::window_redraw");
  
  for (list::size_type i = 0; i < _window_list.size(); ++i)
    _window_list[i]->redraw();
}

void Global::window_update()
{
  DJV_DEBUG("Global::window_update");
  
  for (list::size_type i = 0; i < _window_list.size(); ++i)
  {
    _window_list[i]->dirty();
    _window_list[i]->update();
  }
}

void Global::window_hide()
{
  // Close all windows.
  
  for (list::size_type i = 0; i < _window_list.size(); ++i)
    _window_list[i]->hide();
}

//------------------------------------------------------------------------------

}

