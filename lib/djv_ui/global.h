/*------------------------------------------------------------------------------
 lib/djv_ui/global.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_GLOBAL_H
#define DJV_UI_GLOBAL_H

#include <djv_ui/global_def.h>

#include <djv_ui/callback.h>
#include <djv_ui/widget_def.h>
#include <djv_ui/window_def.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global options.

class _DJV_UI Global : public Callback_Base
{
public:

  Global();

  ~Global();
  
  // XXX

  void widget_add(Widget_Base *);
  void widget_del(Widget_Base *);
  
  void window_add(Window *);
  void window_del(Window *);
  void window_focus(Window *);
  void window_update();
  void window_redraw();
  void window_hide();
  
  const Widget_Base * widget_get(const Fl_Widget *);
  
  callback::Signal<Window *> window_add_signal;
  callback::Signal<Window *> window_del_signal;
  callback::Signal<Window *> window_focus_signal;

private:

  List<Widget_Base *> _widget_list;
  List<Window *> _window_list;
};

//! Access to global options.

_DJV_UI Global * global();

//@}
//------------------------------------------------------------------------------

}

#endif

