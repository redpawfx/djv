/*------------------------------------------------------------------------------
 bin/djv_view/image_group.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "image_group.h"

#include "image_display_profile.h"
#include "image_prefs.h"
#include "shortcut.h"
#include <djv_ui/choice.h>
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/style.h>
#include <djv_ui/tool_button.h>
#include <djv_image/io.h>

namespace djv_bin {
namespace view {
namespace image {

//------------------------------------------------------------------------------
// Group::_Group
//------------------------------------------------------------------------------

struct Group::_Group
{
  djv_ui::menu::Menu * menu;
  List<int> menu_scale;
  List<int> menu_rotate;
  List<int> menu_display_profile;
  List<int> menu_channel;
  
  djv_ui::Tool_Button * display_profile;
};

//------------------------------------------------------------------------------
// Group
//------------------------------------------------------------------------------

Group::Group(
  djv_ui::menu::Menu * menu,
  djv_ui::Layout_H * toolbar,
  const Group * copy
) :
  redraw_signal(this),
  resize_signal(this),
  frame_store_signal(this),
  display_profile_signal(this),
  display_profile_value_signal(this),
  update_signal(this),
  _frame_store(copy ? copy->_frame_store : false),
  _mirror(copy ? copy->_mirror : prefs()->mirror()),
  _scale(copy ? copy->_scale : prefs()->scale()),
  _rotate(copy ? copy->_rotate : prefs()->rotate()),
  _color_profile(copy ? copy->_color_profile : prefs()->color_profile()),
  _display_profile(
    copy ? copy->_display_profile : prefs()->display_profile_value()
  ),
  _channel(copy ? copy->_channel : prefs()->channel()),
  _p(new _Group)
{
  using namespace djv_ui;
  
  DJV_DEBUG("Group::Group");
  
  // Create widgets.
  
  _p->menu = menu;
  
  _p->display_profile =
    new Tool_Button("color_wheel");
  
  // Layout.

  toolbar->add(_p->display_profile);
  
  // Callbacks.
  
  _p->display_profile->signal.set(this, display_profile_callback);
  
  prefs()->mirror_signal.set(this, mirror_callback);
  prefs()->scale_signal.set(this, scale_callback);
  prefs()->rotate_signal.set(this, rotate_callback);
  prefs()->color_profile_signal.set(this, color_profile_callback);
  prefs()->display_profile_value_signal.set(this, display_profile_callback);
  prefs()->display_profile_list_signal.set(this, display_profile_list_callback);
  prefs()->channel_signal.set(this, channel_callback);
}

Group::~Group()
{}

void Group::display_profile(const Display_Profile & in)
{
  _display_profile = in;
}

void Group::menu_update(djv_ui::menu::Group * in)
{
  using namespace djv_ui::menu;
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
    
  // Menu:
  //
  // * Frame Store
  // * Load Frame Store
  //   ---
  // * Mirror Horizontal
  // * Mirror Vertical
  // * Scale
  //   * ...
  // * Rotate
  //   * ...
  //   ---
  // * Color Profile
  // * Display Profile
  //   * Show
  //   * Reset
  //     ---
  //   * Add
  //   * Delete
  //   * Delete All
  //     ---
  //   * ...
  //   ---
  // * Channel
  //   * ...
  
  in->add(menu_title, 0, 0, 0, SUB_MENU);

  in->add(
    menu_frame_store,
    shortcut[shortcut::IMAGE_FRAME_STORE].value,
    _frame_store_callback,
    this,
    TOGGLE,
    _frame_store
  );

  in->add(
    menu_frame_store_load,
    shortcut[shortcut::IMAGE_FRAME_STORE_LOAD].value,
    _frame_store_load_callback,
    this,
    DIVIDER
  );

  in->add(
    menu_mirror_h,
    shortcut[shortcut::IMAGE_MIRROR_HORIZONTAL].value,
    _mirror_h_callback,
    this,
    TOGGLE,
    _mirror.x
  );

  in->add(
    menu_mirror_v,
    shortcut[shortcut::IMAGE_MIRROR_VERTICAL].value,
    _mirror_v_callback,
    this,
    TOGGLE,
    _mirror.y
  );
  
  in->add(menu_scale, 0, 0, 0, SUB_MENU);
  _p->menu_scale = in->add(
    label_scale(),
    List<int>() <<
      shortcut[shortcut::IMAGE_SCALE_NONE].value <<
      shortcut[shortcut::IMAGE_SCALE_16_9].value <<
      shortcut[shortcut::IMAGE_SCALE_1_85].value <<
      shortcut[shortcut::IMAGE_SCALE_2_35].value,
    _scale_callback,
    this,
    RADIO,
    _scale
  );
  in->end();
  
  in->add(menu_rotate, 0, 0, 0, SUB_MENU | DIVIDER);
  _p->menu_rotate = in->add(
    label_rotate(),
    List<int>() <<
      shortcut[shortcut::IMAGE_ROTATE_0].value <<
      shortcut[shortcut::IMAGE_ROTATE_90].value <<
      shortcut[shortcut::IMAGE_ROTATE_180].value <<
      shortcut[shortcut::IMAGE_ROTATE_270].value,
    _rotate_callback,
    this,
    RADIO,
    _rotate
  );
  in->end();

  in->add(
    menu_color_profile,
    shortcut[shortcut::IMAGE_COLOR_PROFILE].value,
    _color_profile_callback,
    this,
    TOGGLE,
    _color_profile
  );
  
  in->add(menu_display_profile, 0, 0, 0, SUB_MENU | DIVIDER);

  in->add(
    menu_display_profile_show,
    shortcut[shortcut::IMAGE_DISPLAY_PROFILE].value,
    _display_profile_callback,
    this
  );
  
  in->add(
    menu_display_profile_reset,
    shortcut[shortcut::IMAGE_DISPLAY_PROFILE_RESET].value,
    _display_profile_reset_callback,
    this,
    DIVIDER
  );
  
  in->add(menu_display_profile_add, 0, _display_profile_add_callback, this);
  in->add(menu_display_profile_del, 0, _display_profile_del_callback, this);
  
  in->add(
    menu_display_profile_del_all,
    0,
    _display_profile_del_all_callback,
    this,
    DIVIDER
  );
  
  _p->menu_display_profile = in->add(
    prefs()->display_profile_name_list(),
    List<int>() <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_1].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_2].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_3].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_4].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_5].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_6].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_7].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_8].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_9].value <<
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE_10].value,
    _display_profile_list_callback,
    this
  );
  in->end();
  
  in->add(menu_channel, 0, 0, 0, SUB_MENU);
  _p->menu_channel = in->add(
    label_channel(),
    List<int>() <<
      0 <<
      shortcut[shortcut::IMAGE_CHANNEL_RED].value <<
      shortcut[shortcut::IMAGE_CHANNEL_GREEN].value <<
      shortcut[shortcut::IMAGE_CHANNEL_BLUE].value <<
      shortcut[shortcut::IMAGE_CHANNEL_ALPHA].value,
    _channel_callback,
    this,
    RADIO,
    _channel
  );
  in->end();
  
  in->end();
}

void Group::toolbar_update()
{
  // Update tooltips.
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();

  _p->display_profile->tooltip(string::Format(tooltip_display_profile).
    arg(djv_ui::shortcut::label(
      shortcut[shortcut::IMAGE_DISPLAY_PROFILE].value)));
}

bool Group::frame_store() const
{
  return _frame_store;
}

void Group::frame_store_callback(bool in)
{
  DJV_DEBUG("Group::frame_store_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  _frame_store = in;
  
  redraw_signal.emit(true);
  update_signal.emit(true);
}

void Group::frame_store_load_callback(bool)
{
  DJV_DEBUG("Group::frame_store_load_callback");
  
  frame_store_signal.emit(true);
}

void Group::_frame_store_callback()
{
  frame_store_callback(_p->menu->value());
}

void Group::_frame_store_load_callback()
{
  frame_store_load_callback(true);
}

const V2b & Group::mirror() const
{
  return _mirror;
}

void Group::mirror_callback(const V2b & in)
{
  DJV_DEBUG("Group::mirror_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  _mirror = in;
  
  redraw_signal.emit(true);
  update_signal.emit(true);
}

void Group::_mirror_h_callback()
{
  mirror_callback(V2b(_p->menu->value(), _mirror.y));
}

void Group::_mirror_v_callback()
{
  mirror_callback(V2b(_mirror.x, _p->menu->value()));
}

SCALE Group::scale() const
{
  return _scale;
}

void Group::scale_callback(SCALE in)
{
  DJV_DEBUG("Group::scale_callback");
  //DJV_DEBUG_PRINT("in = " << in);
  
  _scale = in;
  
  redraw_signal.emit(true);
  resize_signal.emit(true);
  update_signal.emit(true);
}

void Group::_scale_callback()
{
  scale_callback(SCALE(list::find(_p->menu->item(), _p->menu_scale)));
}

ROTATE Group::rotate() const
{
  return _rotate;
}

void Group::rotate_callback(ROTATE in)
{
  DJV_DEBUG("Group::rotate_callback");
  //DJV_DEBUG_PRINT("in = " << in);
  
  _rotate = in;
  
  redraw_signal.emit(true);
  resize_signal.emit(true);
  update_signal.emit(true);
}

void Group::_rotate_callback()
{
  rotate_callback(ROTATE(list::find(_p->menu->item(), _p->menu_rotate)));
}

bool Group::color_profile() const
{
  return _color_profile;
}

void Group::color_profile_callback(bool in)
{
  DJV_DEBUG("Group::color_profile_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  _color_profile = in;
  
  redraw_signal.emit(true);
  update_signal.emit(true);
}

void Group::_color_profile_callback()
{
  color_profile_callback(_p->menu->value());
}

const Display_Profile & Group::display_profile() const
{
  return _display_profile;
}

void Group::display_profile_callback(bool)
{
  display_profile_signal.emit(true);
}

void Group::display_profile_callback(const Display_Profile & in)
{
  display_profile_value_signal.emit(in);
}

void Group::_display_profile_callback()
{
  display_profile_callback(true);
}

void Group::display_profile_reset_callback(bool)
{
  Display_Profile tmp;
  
  display_profile_callback(tmp);
}

void Group::_display_profile_reset_callback()
{
  display_profile_reset_callback(true);
}

void Group::display_profile_add_callback(const String & in)
{
  DJV_DEBUG("Group::display_profile_add_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  List<Display_Profile> list = prefs()->display_profile_list();
  Display_Profile tmp = _display_profile;
  tmp.name = in;
  list += tmp;
  prefs()->display_profile_list(list);
}

void Group::_display_profile_add_callback()
{
  djv_ui::dialog::global()->input(
    dialog_display_profile_add,
    "",
    this,
    display_profile_add_callback
  );
}

void Group::display_profile_del_callback(const List<int> & in)
{
  DJV_DEBUG("Group::display_profile_del_callback");
  DJV_DEBUG_PRINT("in = " << in);
  
  const List<Display_Profile> & list = prefs()->display_profile_list();
  
  List<Display_Profile> tmp;
  for (list::size_type i = 0; i < list.size(); ++i)
  {
    list::size_type j = 0;
    for (; j < in.size(); ++j)
      if (in[j] == i)
        break;
    if (j < in.size())
      continue;
    tmp += list[i];
  }
  
  prefs()->display_profile_list(tmp);
}

void Group::_display_profile_del_callback()
{
  const List<String> list = prefs()->display_profile_name_list();
  if (! list.size())
    return;
  
  djv_ui::dialog::global()->multi_choice(
    dialog_display_profile_del,
    list,
    this,
    display_profile_del_callback
  );
}

void Group::display_profile_del_all_callback(bool)
{
  prefs()->display_profile_list(List<Display_Profile>());
}

void Group::_display_profile_del_all_callback()
{
  const List<String> list = prefs()->display_profile_name_list();
  if (! list.size())
    return;
  
  djv_ui::dialog::global()->question(
    dialog_display_profile_del_all,
    this,
    display_profile_del_all_callback
  );
}

void Group::display_profile_list_callback(bool)
{
  DJV_DEBUG("Group::display_profile_list_callback");
  
  update_signal.emit(true);
}

void Group::_display_profile_list_callback()
{
  display_profile_callback(
    prefs()->display_profile_list()[
      list::find(_p->menu->item(), _p->menu_display_profile)
    ]
  );
}

CHANNEL Group::channel() const
{
  return _channel;
}

void Group::channel_callback(CHANNEL in)
{
  DJV_DEBUG("Group::channel_callback");
  DJV_DEBUG_PRINT("in = " << in);

  if (in == _channel)
    _channel = djv_image::gl_data::CHANNEL_DEFAULT;
  else
    _channel = in;
  
  redraw_signal.emit(true);
  update_signal.emit(true);
}

void Group::_channel_callback()
{
  channel_callback(CHANNEL(list::find(_p->menu->item(), _p->menu_channel)));
}

//------------------------------------------------------------------------------

}
}
}

