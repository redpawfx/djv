/*------------------------------------------------------------------------------
 lib/djv_ui/image_io.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_io.h"

#include <djv_ui/layout_row.h>
#include <djv_ui/tabs.h>

namespace djv_ui {
namespace image_io {

//------------------------------------------------------------------------------
// Prefs_Widget
//------------------------------------------------------------------------------

Prefs_Widget::Prefs_Widget() :
  signal(this)
{
  // Create widgets.
  
  tabs::Tabs * tabs_widget =
    new tabs::Tabs;
  
  const List<djv_image::io::Global::Group> & list =
    djv_image::io::global()->group_list();
  
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    image_io::Base * base = global()->base()->get(list[i].base);
    Load * load = global()->load()->get(list[i].load);
    Save * save = global()->save()->get(list[i].save);

    tabs::Tabs * tabs_sub_widget = 0;
    if (base || load || save)
    {
      tabs_sub_widget = new tabs::Tabs;
    
      tabs_widget->tab_add(list[i].name, tabs_sub_widget);
    }
    
    if (base)
    {
      base->set(list[i].base);
      base->signal.set(this, widget_callback);
      tabs_sub_widget->tab_add(prefs_base_title, base);
    }
    if (load)
    {
      load->set(list[i].load);
      load->signal.set(this, widget_callback);
      tabs_sub_widget->tab_add(prefs_load_title, load);
    }
    if (save)
    {
      save->set(list[i].save);
      save->signal.set(this, widget_callback);
      tabs_sub_widget->tab_add(prefs_save_title, save);
    }
  }
  
  // Layout.
  
  Layout_V * layout = new Layout_V(this);
  layout->margin(0);
  layout->add(tabs_widget);
}

void Prefs_Widget::widget_callback(djv_image::io::Base * in)
{
  DJV_DEBUG("Prefs_Widget::widget_callback");
  DJV_DEBUG_PRINT("in = " << in->name());
  
  signal.emit(in);
  global()->signal.emit(in);
}

//------------------------------------------------------------------------------

}
}
