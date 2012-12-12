/*------------------------------------------------------------------------------
 bin/djv_view/tool_group.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "tool_group.h"

#include "tool_color_picker.h"
#include "tool_histogram.h"
#include "tool_info.h"
#include "tool_magnify.h"
#include "shortcut.h"
#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/style.h>
#include <djv_ui/tool_button.h>
#include <djv_image/io.h>

namespace djv_bin {
namespace view {
namespace tool {

//------------------------------------------------------------------------------
// Group::_Group
//------------------------------------------------------------------------------

struct Group::_Group
{
  djv_ui::menu::Menu * menu;

  djv_ui::Tool_Button * magnify;
  djv_ui::Tool_Button * color_picker;
  djv_ui::Tool_Button * histogram;
  djv_ui::Tool_Button * info;
};

//------------------------------------------------------------------------------
// Group
//------------------------------------------------------------------------------

Group::Group(
  djv_ui::menu::Menu * menu,
  djv_ui::Layout_H * toolbar,
  const Group * copy
) :
  magnify_signal(this),
  color_picker_signal(this),
  histogram_signal(this),
  info_signal(this),
  _p(new _Group)
{
  using namespace djv_ui;
  
  DJV_DEBUG("Group::Group");
  
  // Create widgets.
  
  _p->menu = menu;
  
  _p->magnify =
    new Tool_Button("magnify");
  
  _p->color_picker =
    new Tool_Button("color_picker");
  
  _p->histogram =
    new Tool_Button("histogram");
  
  _p->info =
    new Tool_Button("info");
  
  // Layout.
  
  toolbar->add(new Frame_Group_H(List<Frame *>() <<
    _p->magnify <<
    _p->color_picker <<
    _p->histogram <<
    _p->info));
  
  // Initialize.
  
  // Callbacks.
  
  _p->magnify->signal.set(this, magnify_callback);
  _p->color_picker->signal.set(this, color_picker_callback);
  _p->histogram->signal.set(this, histogram_callback);
  _p->info->signal.set(this, info_callback);
}

Group::~Group()
{}

void Group::menu_update(djv_ui::menu::Group * in)
{
  using namespace djv_ui::menu;
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
    
  // Menu:
  //
  // * Magnify
  // * Color Picker
  // * Histogram
  // * Information
  
  in->add(menu_title, 0, 0, 0, SUB_MENU);
  
  in->add(
    menu_magnify,
    shortcut[shortcut::TOOL_MAGNIFY].value,
    _magnify_callback,
    this
  );
  
  in->add(
    menu_color_picker,
    shortcut[shortcut::TOOL_COLOR_PICKER].value,
    _color_picker_callback,
    this
  );
  
  in->add(
    menu_histogram,
    shortcut[shortcut::TOOL_HISTOGRAM].value,
    _histogram_callback,
    this
  );
  
  in->add(
    menu_info,
    shortcut[shortcut::TOOL_INFO].value,
    _info_callback,
    this
  );
  
  in->end();
}

void Group::toolbar_update()
{
  // Update tooltips.
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
  
  _p->magnify->tooltip(string::Format(tooltip_magnify).
    arg(djv_ui::shortcut::label(shortcut[shortcut::TOOL_MAGNIFY].value)));

  _p->color_picker->tooltip(string::Format(tooltip_color_picker).
    arg(djv_ui::shortcut::label(shortcut[shortcut::TOOL_COLOR_PICKER].value)));

  _p->histogram->tooltip(string::Format(tooltip_histogram).
    arg(djv_ui::shortcut::label(shortcut[shortcut::TOOL_HISTOGRAM].value)));

  _p->info->tooltip(string::Format(tooltip_info).
    arg(djv_ui::shortcut::label(shortcut[shortcut::TOOL_INFO].value)));
}

void Group::magnify_callback(bool)
{
  magnify_signal.emit(true);
}

void Group::_magnify_callback()
{
  magnify_callback(true);
}

void Group::color_picker_callback(bool)
{
  color_picker_signal.emit(true);
}

void Group::_color_picker_callback()
{
  color_picker_callback(true);
}

void Group::histogram_callback(bool)
{
  histogram_signal.emit(true);
}

void Group::_histogram_callback()
{
  histogram_callback(true);
}

void Group::info_callback(bool)
{
  info_signal.emit(true);
}

void Group::_info_callback()
{
  info_callback(true);
}

//------------------------------------------------------------------------------

}
}
}

