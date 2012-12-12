/*------------------------------------------------------------------------------
 bin/djv_view/file_prefs_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_prefs.h"

#include <djv_ui/check_button.h>
#include <djv_ui/choice.h>
#include <djv_ui/group_box.h>
#include <djv_ui/int_value.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/radio_button.h>
#include <djv_ui/style.h>
#include <djv_ui/text.h>

namespace djv_bin {
namespace view {
namespace file {

//------------------------------------------------------------------------------
// Cache_Size_Widget
//------------------------------------------------------------------------------

namespace {

class Cache_Size_Widget : public djv_ui::Widget
{
public:

  Cache_Size_Widget() :
    signal(this)
  {
    using namespace djv_ui;
  
    _widget = new Int_Edit;
    _widget->range(0, _widget->max());
    _widget->inc(100, 1000);
    menu::Popup * menu = new menu::Popup(cache::label_size());
    
    size(layout_item::FIXED, layout_item::FIXED);
    Layout_H * layout = new Layout_H(this);
    layout->margin(0);
    layout->add(_widget);
    layout->add(menu);
    
    _widget->set(prefs()->cache_size());
    
    _widget->signal.set(this, widget_callback);
    menu->signal.set(this, menu_callback);
  }
  
  void set(int in)
  {
    _widget->set(in);
  }
  
  djv_ui::callback::Signal<int> signal;
  
private:

  DJV_CALLBACK(Cache_Size_Widget, widget_callback, int) { signal.emit(in); }
  DJV_CALLBACK(Cache_Size_Widget, menu_callback, int)
    { _widget->set(cache::default_size()[in]); }
  
  djv_ui::Int_Edit * _widget;
};

}

//------------------------------------------------------------------------------
// Prefs_Widget
//------------------------------------------------------------------------------

Prefs_Widget::Prefs_Widget()
{
  using namespace djv_ui;
  
  // Create options widgets.

  group_box::Box_Large * group =
    new group_box::Box_Large(prefs_group);
  
  Check_Button * seq_auto =
    new Check_Button(prefs_seq_auto);

  Check_Button * command_line_combine =
    new Check_Button(prefs_command_line_combine);
  
  // Create proxy scale widgets.
  
  group_box::Box_Large * proxy_group =
    new group_box::Box_Large(prefs_proxy_group);
  Label * proxy_text =
    new Label(prefs_proxy_text);
  
  Radio_Button_Group * proxy =
    new Radio_Button_Group(djv_image::data::label_proxy());

  // Create memory cache widgets.
  
  group_box::Box_Large * cache_group =
    new group_box::Box_Large(prefs_cache_group);
  Label * cache_text =
    new Label(prefs_cache_text);
  
  Check_Button * cache =
    new Check_Button(prefs_cache);
  
  Cache_Size_Widget * cache_size =
    new Cache_Size_Widget;
  Label * cache_size_label =
    new Label(prefs_cache_size);
  
  Radio_Button_Group * cache_type =
    new Radio_Button_Group(prefs_cache_type, cache::label_cache_text());
  
  Check_Button * cache_display =
    new Check_Button(prefs_cache_display);

  // Layout.
  
  Layout_V * layout = 0, * layout_v = 0;
  Layout_H * layout_h = 0;
  
  layout = new Layout_V(this);
  layout->spacing(style::global()->spacing_large());

  layout->add(group);
  group->layout()->add(seq_auto);
  group->layout()->add(command_line_combine);

  layout->add(proxy_group);
  proxy_group->layout()->add(proxy_text);
  proxy_group->layout()->add(proxy);

  layout->add(cache_group);
  cache_group->layout()->add(cache_text);
  cache_group->layout()->add(cache);
  layout_h = new Layout_H(cache_group->layout());
  layout_h->margin(0);
  layout_h->add(cache_size_label);
  layout_h->add(cache_size);
  cache_group->layout()->add(cache_type);
  cache_group->layout()->add(cache_display);

  layout->add_stretch();

  // Initialize.
  
  seq_auto->set(prefs()->seq_auto());
  command_line_combine->set(prefs()->command_line_combine());
  proxy->set(prefs()->proxy());
  cache->set(prefs()->cache());
  cache_size->set(prefs()->cache_size());
  cache_type->set(prefs()->cache_type());
  cache_display->set(prefs()->cache_display());
  
  // Callbacks.

  seq_auto->signal.set(this, seq_auto_callback);
  command_line_combine->signal.set(this, command_line_combine_callback);
  proxy->signal.set(this, proxy_callback);
  cache->signal.set(this, cache_callback);
  cache_size->signal.set(this, cache_size_callback);
  cache_type->signal.set(this, cache_type_callback);
  cache_display->signal.set(this, cache_display_callback);
}

void Prefs_Widget::seq_auto_callback(bool in)
{
  prefs()->seq_auto(in);
}

void Prefs_Widget::command_line_combine_callback(bool in)
{
  prefs()->command_line_combine(in);
}

void Prefs_Widget::proxy_callback(int in)
{
  prefs()->proxy(static_cast<djv_image::data::PROXY>(in));
}

void Prefs_Widget::cache_callback(bool in)
{
  prefs()->cache(in);
}

void Prefs_Widget::cache_size_callback(int in)
{
  prefs()->cache_size(in);
}

void Prefs_Widget::cache_type_callback(int in)
{
  prefs()->cache_type(static_cast<cache::CACHE>(in));
}

void Prefs_Widget::cache_display_callback(bool in)
{
  prefs()->cache_display(in);
}

//------------------------------------------------------------------------------

}
}
}

