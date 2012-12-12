/*------------------------------------------------------------------------------
 lib/djv_ui/choice_edit.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "choice.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/style.h>
#include <djv_ui/text.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv_ui {
namespace choice {

//------------------------------------------------------------------------------
// Choice_Edit::_Choice_Edit
//------------------------------------------------------------------------------

struct Choice_Edit::_Choice_Edit
{
  Text_Edit * widget;
  
  menu::Popup * menu;
};

//------------------------------------------------------------------------------
// Choice_Edit
//------------------------------------------------------------------------------

Choice_Edit::Choice_Edit(const List<String> & list) :
  signal(this),
  _list(list),
  _p(new _Choice_Edit)
{
  debug_path("Choice_Edit");
  
  // Create widgets.
  
  _p->widget = new Text_Edit;
  _p->widget->style(frame::STYLE_EMPTY);
  
  _p->menu = new menu::Popup;

  // Layout.

  size(layout_item::FIXED, layout_item::FIXED);
  align(layout_item::LEFT, layout_item::CENTER);
  
  Layout_H * layout = new Layout_H(this);
  layout->spacing(2);
  layout->margin(0);
  layout->add(_p->widget);
  layout->stretch(_p->widget);
  layout->add(_p->menu);
  
  // Initialize.
  
  style(frame::STYLE_BORDER);
  
  list_update();

  // Callbacks.

  _p->widget->signal.set(this, widget_callback);
  _p->menu->signal.set(this, menu_callback);
}

Choice_Edit::~Choice_Edit()
{}

void Choice_Edit::set(const String & in)
{
  _p->widget->set(in);
}
  
String Choice_Edit::get() const
{
  return _p->widget->get();
}

void Choice_Edit::list(const List<String> & in)
{
  if (in == _list)
    return;
  
  _list = in;
  
  list_update();
}

const List<String> & Choice_Edit::list() const
{
  return _list;
}

void Choice_Edit::size_string(const String & in)
{
  _p->widget->size_string(in);
}

void Choice_Edit::dirty()
{
  Frame::dirty();
}

void Choice_Edit::widget_callback(const String & in)
{
  signal.emit(in);
}

void Choice_Edit::menu_callback(int in)
{
  set(_list[in]);
}

void Choice_Edit::list_update()
{
  _p->menu->set(_list);
  
  // Set to width of largest menu item.
  
  if (_list.size() && ! _p->widget->size_string().size())
  {
    fl_font(text_font(), style::global()->font_size());
    
    list::size_type i = 0;
    for (list::size_type j = 1; j < _list.size(); ++j)
      if (fl_width(_list[j].c_str()) > fl_width(_list[i].c_str()))
        i = j;
    _p->widget->size_string(_list[i] + " ");
    
    update();
  }
}

//------------------------------------------------------------------------------

}
}

