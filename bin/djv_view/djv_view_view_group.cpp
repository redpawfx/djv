//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_view_view_group.cpp

#include <djv_view_view_group.h>

#include <djv_view_shortcut.h>
#include <djv_view_shortcut_prefs.h>
#include <djv_view_view_prefs.h>

#include <djv_frame_group.h>
#include <djv_inc_button.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_menu.h>
#include <djv_style.h>
#include <djv_tool_button.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// View_Group
//------------------------------------------------------------------------------

namespace
{

const String
    toolbar_zoom = "%%%",
    toolbar_zoom_format = "000%",
    tooltip_zoom = "Zoom";

} // namespace

View_Group::View_Group(
    Menu *                menu,
    Abstract_Row_Layout * toolbar,
    const View_Group *    copy) :
    move_signal       (this),
    center_signal     (this),
    zoom_signal       (this),
    zoom_reset_signal (this),
    reset_signal      (this),
    fit_signal        (this),
    overlay_signal    (this),
    update_signal     (this),
    _grid             (View_Prefs::global()->grid()),
    _hud              (View_Prefs::global()->hud()),
    _menu             (0),
    _zoom_in_widget   (0),
    _zoom_out_widget  (0),
    _zoom_reset_widget(0),
    _fit_widget       (0),
    _zoom_label       (0)
{
    if (copy)
    {
        _grid = copy->_grid;
        _hud = copy->_hud;
    }

    // Create widgets.

    _menu = menu;

    _zoom_in_widget = new Tool_Button("view_zoom_in");
    _zoom_in_widget->type(Tool_Button::REPEAT);

    _zoom_out_widget = new Tool_Button("view_zoom_out");
    _zoom_out_widget->type(Tool_Button::REPEAT);

    _zoom_reset_widget = new Tool_Button("view_zoom_reset");

    _fit_widget = new Tool_Button("view_fit");

    _zoom_label = new Label;
    _zoom_label->size_string(toolbar_zoom_format);
    _zoom_label->tooltip(tooltip_zoom);
    _zoom_label->align(Layout_Item::RIGHT, Layout_Item::CENTER);

    Inc_Button * zoom_inc_widget = new Inc_Button;

    // Layout.

    toolbar->add(new Horizontal_Frame_Group(List<Frame *>() <<
        _zoom_in_widget <<
        _zoom_out_widget <<
        _zoom_reset_widget <<
        _fit_widget));
    toolbar->add(_zoom_label);
    toolbar->add(zoom_inc_widget);

    // Callbacks.

    _zoom_in_widget->signal.set(this, zoom_in_callback);
    _zoom_out_widget->signal.set(this, zoom_out_callback);
    zoom_inc_widget->inc_signal.set(this, zoom_inc_callback);
    zoom_inc_widget->dec_signal.set(this, zoom_dec_callback);
    _zoom_reset_widget->signal.set(this, zoom_reset_callback);
    _fit_widget->signal.set(this, fit_callback);

    View_Prefs::global()->grid_signal.set(this, grid_callback);
    View_Prefs::global()->hud_signal.set(this, hud_callback);
}

View_Group::~View_Group()
{}

void View_Group::zoom(double in)
{
    _zoom_label->set(
        String_Format(toolbar_zoom).
        arg(static_cast<int>(in * 100)));
}

namespace
{

const String
    menu_title = "View",
    menu_left = "&Left",
    menu_right = "&Right",
    menu_up = "&Up",
    menu_down = "&Down",
    menu_center = "&Center",
    menu_zoom_in = "Zoom &In",
    menu_zoom_out = "Zoom &Out",
    menu_zoom_reset = "Zoom Rese&t",
    menu_reset = "R&eset",
    menu_fit = "&Fit",
    menu_grid = "&Grid",
    menu_hud = "&HUD";

} // namespace

void View_Group::menu_update(Menu_Item_Group * in)
{
    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

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

    in->add(menu_title, 0, 0, 0, Menu_Item::SUB_MENU);

    in->add(
        menu_left,
        shortcuts[Shortcut::VIEW_LEFT].value,
        _left_callback,
        this);

    in->add(
        menu_right,
        shortcuts[Shortcut::VIEW_RIGHT].value,
        _right_callback,
        this);

    in->add(
        menu_up,
        shortcuts[Shortcut::VIEW_UP].value,
        _up_callback,
        this);

    in->add(
        menu_down,
        shortcuts[Shortcut::VIEW_DOWN].value,
        _down_callback,
        this);

    in->add(
        menu_center,
        shortcuts[Shortcut::VIEW_CENTER].value,
        _center_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(
        menu_zoom_in,
        shortcuts[Shortcut::VIEW_ZOOM_IN].value,
        _zoom_in_callback,
        this);

    in->add(
        menu_zoom_out,
        shortcuts[Shortcut::VIEW_ZOOM_OUT].value,
        _zoom_out_callback,
        this);

    in->add(
        menu_zoom_reset,
        shortcuts[Shortcut::VIEW_ZOOM_RESET].value,
        _zoom_reset_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(
        menu_reset,
        shortcuts[Shortcut::VIEW_RESET].value,
        _reset_callback,
        this);

    in->add(
        menu_fit,
        shortcuts[Shortcut::VIEW_FIT].value,
        _fit_callback,
        this,
        Menu_Item::DIVIDER);

    in->add(menu_grid, 0, 0, 0, Menu_Item::SUB_MENU);
    
    _menu_grid = in->add(
        View::label_grid(),
        List<int>(),   
        _grid_callback,
        this,
        Menu_Item::RADIO,
        _grid);
    
    in->end();

    in->add(
        menu_hud,
        shortcuts[Shortcut::VIEW_HUD].value,
        _hud_callback,
        this,
        Menu_Item::TOGGLE,
        _hud);

    in->end();
}

namespace
{

const String
    tooltip_zoom_in = "Zoom in\n\nShortcut: %%",
    tooltip_zoom_out = "Zoom out\n\nShortcut: %%",
    tooltip_zoom_reset = "Reset zoom\n\nShortcut: %%",
    tooltip_fit = "Fit image to window\n\nShortcut: %%";

} // namespace

void View_Group::toolbar_update()
{
    // Update tooltips.

    const List<djv::Shortcut> & shortcuts =
        Shortcut_Prefs::global()->shortcuts();

    _zoom_in_widget->tooltip(String_Format(tooltip_zoom_in).
        arg(djv::Shortcut::label(shortcuts[Shortcut::VIEW_ZOOM_IN].value)));

    _zoom_out_widget->tooltip(String_Format(tooltip_zoom_out).
        arg(djv::Shortcut::label(shortcuts[Shortcut::VIEW_ZOOM_OUT].value)));

    _zoom_reset_widget->tooltip(String_Format(tooltip_zoom_reset).
        arg(djv::Shortcut::label(shortcuts[Shortcut::VIEW_ZOOM_RESET].value)));

    _fit_widget->tooltip(String_Format(tooltip_fit).
        arg(djv::Shortcut::label(shortcuts[Shortcut::VIEW_FIT].value)));
}

void View_Group::_left_callback()
{
    move_signal.emit(V2i(10, 0));
}

void View_Group::_right_callback()
{
    move_signal.emit(V2i(-10, 0));
}

void View_Group::_up_callback()
{
    move_signal.emit(V2i(0, 10));
}

void View_Group::_down_callback()
{
    move_signal.emit(V2i(0, -10));
}

void View_Group::_center_callback()
{
    center_signal.emit(true);
}

void View_Group::zoom_in_callback(bool)
{
    zoom_signal.emit(2.0);
}

void View_Group::zoom_out_callback(bool)
{
    zoom_signal.emit(0.5);
}

void View_Group::zoom_inc_callback(bool)
{
    zoom_signal.emit(1.1);
}

void View_Group::zoom_dec_callback(bool)
{
    zoom_signal.emit(0.9);
}

void View_Group::zoom_reset_callback(bool)
{
    zoom_reset_signal.emit(true);
}

void View_Group::_zoom_in_callback()
{
    zoom_in_callback(true);
}

void View_Group::_zoom_out_callback()
{
    zoom_out_callback(true);
}

void View_Group::_zoom_reset_callback()
{
    zoom_reset_callback(true);
}

void View_Group::_reset_callback()
{
    reset_signal.emit(true);
}

void View_Group::fit_callback(bool)
{
    fit_signal.emit(true);
}

void View_Group::_fit_callback()
{
    fit_callback(true);
}

void View_Group::grid(View::GRID in)
{
    if (in == _grid)
    {
        return;
    }

    _grid = in;

    overlay_signal.emit(true);
    update_signal.emit(true);
}

View::GRID View_Group::grid() const
{
    return _grid;
}

void View_Group::grid_callback(View::GRID in)
{
    grid(in);
}

void View_Group::_grid_callback()
{
    grid(static_cast<View::GRID>(
             List_Util::find(_menu->item(), _menu_grid)));
}

void View_Group::hud(bool in)
{
    if (in == _hud)
    {
        return;
    }

    _hud = in;

    overlay_signal.emit(true);
    update_signal.emit(true);
}

bool View_Group::hud() const
{
    return _hud;
}

void View_Group::hud_callback(bool in)
{
    hud(in);
}

void View_Group::_hud_callback()
{
    hud(_menu->value());
}

} // djv_view

