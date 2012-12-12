/*------------------------------------------------------------------------------
 bin/djv_view/view_group.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "view_group.h"

#include "shortcut.h"
#include "view_prefs.h"
#include <djv_ui/label.h>
#include <djv_ui/layout_row.h>
#include <djv_ui/menu.h>
#include <djv_ui/shortcut.h>
#include <djv_ui/style.h>
#include <djv_ui/tool_button.h>
#include <djv_ui/value.h>

namespace djv_bin {
namespace view {
namespace view {

//------------------------------------------------------------------------------
// Group::_Group
//------------------------------------------------------------------------------

struct Group::_Group
{
  djv_ui::menu::Menu * menu;
  List<int> menu_grid;
  
  djv_ui::Tool_Button * zoom_in;
  djv_ui::Tool_Button * zoom_out;
  djv_ui::Tool_Button * zoom_reset;
  djv_ui::Tool_Button * fit;
  djv_ui::Label * zoom_label;
};

//------------------------------------------------------------------------------
// Group
//------------------------------------------------------------------------------

Group::Group(
  djv_ui::menu::Menu * menu,
  djv_ui::Layout_H * toolbar,
  const Group * copy
) :
  move_signal(this),
  center_signal(this),
  zoom_signal(this),
  zoom_reset_signal(this),
  reset_signal(this),
  fit_signal(this),
  overlay_signal(this),
  update_signal(this),
  _grid(copy ? copy->_grid : prefs()->grid()),
  _hud(copy ? copy->_hud : prefs()->hud()),
  _p(new _Group)
{
  using namespace djv_ui;
  
  // Create widgets.
  
  _p->menu = menu;
  
  _p->zoom_in =
    new Tool_Button("view_zoom_in");
  _p->zoom_in->type(tool_button::REPEAT);
  
  _p->zoom_out =
    new Tool_Button("view_zoom_out");
  _p->zoom_out->type(tool_button::REPEAT);
  
  _p->zoom_reset =
    new Tool_Button("view_zoom_reset");
  
  _p->fit =
    new Tool_Button("view_fit");

  _p->zoom_label =
    new Label;
  _p->zoom_label->size_string(toolbar_zoom_format);
  _p->zoom_label->tooltip(tooltip_zoom);
  _p->zoom_label->align(layout_item::RIGHT, layout_item::CENTER);

  value::Inc_Tool * zoom_inc_widget =
    new value::Inc_Tool;

  // Layout.
  
  toolbar->add(new Frame_Group_H(List<Frame *>() <<
    _p->zoom_in <<
    _p->zoom_out <<
    _p->zoom_reset <<
    _p->fit));
  toolbar->add(_p->zoom_label);
  toolbar->add(zoom_inc_widget);
  
  // Callbacks.
  
  _p->zoom_in->signal.set(this, zoom_in_callback);
  _p->zoom_out->signal.set(this, zoom_out_callback);
  zoom_inc_widget->inc_signal.set(this, zoom_inc_callback);
  zoom_inc_widget->dec_signal.set(this, zoom_dec_callback);
  _p->zoom_reset->signal.set(this, zoom_reset_callback);
  _p->fit->signal.set(this, fit_callback);
  
  prefs()->grid_signal.set(this, grid_callback);
  prefs()->hud_signal.set(this, hud_callback);
}

Group::~Group()
{}

void Group::zoom(double in)
{
  _p->zoom_label->set(
    string::Format(toolbar_zoom).
      arg(static_cast<int>(in * 100))
  );
}

void Group::menu_update(djv_ui::menu::Group * in)
{
  using namespace djv_ui::menu;
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
  
  // Menu:
  //
  // * Left
  // * Right
  // * Up
  // * Down
  // * Center
  //   ---
  // * Zoom In
  // * Zoom Out
  // * Zoom Reset
  //   ---
  // * Reset
  // * Fit
  //   ---
  // * Grid
  //   * ...
  // * HUD
    
  in->add(menu_title, 0, 0, 0, SUB_MENU);
  
  in->add(
    menu_left,
    shortcut[shortcut::VIEW_LEFT].value,
    _left_callback,
    this
  );
  
  in->add(
    menu_right,
    shortcut[shortcut::VIEW_RIGHT].value,
    _right_callback,
    this
  );
  
  in->add(
    menu_up,
    shortcut[shortcut::VIEW_UP].value,
    _up_callback,
    this
  );
  
  in->add(
    menu_down,
    shortcut[shortcut::VIEW_DOWN].value,
    _down_callback,
    this
  );
  
  in->add(
    menu_center,
    shortcut[shortcut::VIEW_CENTER].value,
    _center_callback,
    this,
    DIVIDER
  );
  
  in->add(
    menu_zoom_in,
    shortcut[shortcut::VIEW_ZOOM_IN].value,
    _zoom_in_callback,
    this
  );
  
  in->add(
    menu_zoom_out,
    shortcut[shortcut::VIEW_ZOOM_OUT].value,
    _zoom_out_callback,
    this
  );
  
  in->add(
    menu_zoom_reset,
    shortcut[shortcut::VIEW_ZOOM_RESET].value,
    _zoom_reset_callback,
    this,
    DIVIDER
  );
  
  in->add(
    menu_reset,
    shortcut[shortcut::VIEW_RESET].value,
    _reset_callback,
    this
  );
  
  in->add(
    menu_fit,
    shortcut[shortcut::VIEW_FIT].value,
    _fit_callback,
    this,
    DIVIDER
  );
  
  in->add(menu_grid, 0, 0, 0, SUB_MENU);
  _p->menu_grid = in->add(
    label_grid(),
    List<int>(),
    _grid_callback,
    this,
    RADIO,
    _grid
  );
  in->end();
  
  in->add(
    menu_hud,
    shortcut[shortcut::VIEW_HUD].value,
    _hud_callback,
    this,
    TOGGLE,
    _hud
  );
    
  in->end();
}

void Group::toolbar_update()
{
  // Update tooltips.
  
  const List<djv_ui::shortcut::Shortcut> & shortcut =
    shortcut::prefs()->shortcut();
  
  _p->zoom_in->tooltip(string::Format(tooltip_zoom_in).
    arg(djv_ui::shortcut::label(shortcut[shortcut::VIEW_ZOOM_IN].value)));

  _p->zoom_out->tooltip(string::Format(tooltip_zoom_out).
    arg(djv_ui::shortcut::label(shortcut[shortcut::VIEW_ZOOM_OUT].value)));

  _p->zoom_reset->tooltip(string::Format(tooltip_zoom_reset).
    arg(djv_ui::shortcut::label(shortcut[shortcut::VIEW_ZOOM_RESET].value)));

  _p->fit->tooltip(string::Format(tooltip_fit).
    arg(djv_ui::shortcut::label(shortcut[shortcut::VIEW_FIT].value)));
}

void Group::_left_callback()
{
  move_signal.emit(V2i(10, 0));
}

void Group::_right_callback()
{
  move_signal.emit(V2i(-10, 0));
}

void Group::_up_callback()
{
  move_signal.emit(V2i(0, 10));
}

void Group::_down_callback()
{
  move_signal.emit(V2i(0, -10));
}

void Group::_center_callback()
{
  center_signal.emit(true);
}

void Group::zoom_in_callback(bool)
{
  zoom_signal.emit(2.0);
}

void Group::zoom_out_callback(bool)
{
  zoom_signal.emit(0.5);
}

void Group::zoom_inc_callback(bool)
{
  zoom_signal.emit(1.1);
}

void Group::zoom_dec_callback(bool)
{
  zoom_signal.emit(0.9);
}

void Group::zoom_reset_callback(bool)
{
  zoom_reset_signal.emit(true);
}

void Group::_zoom_in_callback()
{
  zoom_in_callback(true);
}

void Group::_zoom_out_callback()
{
  zoom_out_callback(true);
}

void Group::_zoom_reset_callback()
{
  zoom_reset_callback(true);
}

void Group::_reset_callback()
{
  reset_signal.emit(true);
}

void Group::fit_callback(bool)
{
  fit_signal.emit(true);
}

void Group::_fit_callback()
{
  fit_callback(true);
}

void Group::grid(GRID in)
{
  if (in == _grid)
    return;
  
  _grid = in;
  
  overlay_signal.emit(true);
  update_signal.emit(true);
}

GRID Group::grid() const
{
  return _grid;
}

void Group::grid_callback(GRID in)
{
  grid(in);
}

void Group::_grid_callback()
{
  grid(GRID(list::find(_p->menu->item(), _p->menu_grid)));
}

void Group::hud(bool in)
{
  if (in == _hud)
    return;
  
  _hud = in;
  
  overlay_signal.emit(true);
  update_signal.emit(true);
}

bool Group::hud() const
{
  return _hud;
}

void Group::hud_callback(bool in)
{
  hud(in);
}

void Group::_hud_callback()
{
  hud(_p->menu->value());
}

//------------------------------------------------------------------------------

}
}
}

