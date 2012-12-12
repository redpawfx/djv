/*------------------------------------------------------------------------------
 lib/djv_ui/file.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "file.h"

#include <djv_ui/file_browser.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/push_button.h>
#include <djv_ui/text.h>
#include <algorithm>

namespace djv_ui {
namespace file {

//------------------------------------------------------------------------------
// File_Edit::_File_Edit
//------------------------------------------------------------------------------

struct File_Edit::_File_Edit
{
  Text_Edit * widget;
};

//------------------------------------------------------------------------------
// File_Edit
//------------------------------------------------------------------------------

File_Edit::File_Edit() :
  signal(this),
  _p(new _File_Edit)
{
  // Create widgets.
  
  _p->widget = new Text_Edit;
  
  Push_Button * dialog_widget = new Push_Button(label_browse);
  
  // Layout.
  
  Layout_H * layout = new Layout_H(this);
  layout->margin(0);
  
  layout->add(_p->widget);
  layout->stretch(_p->widget);
  
  layout->add(dialog_widget);
  
  // Initialize.
  
  widget_update();

  // Callbacks.
  
  _p->widget->signal.set(this, widget_callback);
  dialog_widget->signal.set(this, dialog_callback);
}

File_Edit::~File_Edit()
{}

void File_Edit::del()
{
  file_browser::del(this);
  
  widget::Widget::del();
}

void File_Edit::set(const File & in)
{
  if (in == _value)
    return;
  
  _value = in;
  
  widget_update();
  
  signal.emit(_value);
}

const File & File_Edit::get() const
{
  return _value;
}

void File_Edit::widget_callback(const String & in)
{
  set(in);
}

void File_Edit::dialog_callback(bool)
{
  file_browser::set(this, dialog_callback);
}

void File_Edit::dialog_callback(const File & in)
{
  set(in);
}

void File_Edit::widget_update()
{
  _p->widget->set(_value);
}

//------------------------------------------------------------------------------
// recent()
//------------------------------------------------------------------------------

void recent(const String & in, List<String> & list, int max)
{
  const List<String>::iterator i = std::find(list.begin(), list.end(), in);
  
  if (i == list.end())
  {
    // Insert new item at front of list.
    
    list.insert(list.begin(), in);
    
    while (static_cast<int>(list.size()) > max)
      list.erase(list.end() - 1);
  }
  else
  {
    // Move existing item to front of list.
    
    const String tmp = *i;
    list.erase(i);
    list.insert(list.begin(), tmp);
  }
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

extern const String
  label_browse = "&Browse";

//------------------------------------------------------------------------------

}
}

