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

//! \file djv_view_view_prefs.cpp

#include <djv_view_view_prefs.h>

#include <djv_application.h>
#include <djv_check_button.h>
#include <djv_check_button_group.h>
#include <djv_choice_widget.h>
#include <djv_color_swatch.h>
#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_prefs.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// View_Prefs
//------------------------------------------------------------------------------

View_Prefs::View_Prefs() :
    resize_signal(this),
    grid_signal(this),
    hud_signal(this),
    _background(Color(0.0)),
    _resize(View::RESIZE_NONE),
    _grid(View::GRID_NONE),
    _grid_color(Color(0.25)),
    _hud(false),
    _hud_show(true, View::_HUD_SHOW_SIZE),
    _hud_color(Color(1.0)),
    _hud_background(View::HUD_BACKGROUND_SHADOW),
    _hud_background_color(Color(0.0))
{
    //DJV_DEBUG("View_Prefs::View_Prefs");

    Prefs prefs(Prefs::prefs(), "view");
    Prefs::get_(&prefs, "background", &_background);
    Prefs::get_(&prefs, "resize", &_resize);
    Prefs::get_(&prefs, "grid", &_grid);
    Prefs::get_(&prefs, "grid_color", &_grid_color);
    Prefs::get_(&prefs, "hud", &_hud);
    Prefs::get_(&prefs, "hud_show", &_hud_show);
    Prefs::get_(&prefs, "hud_color", &_hud_color);
    Prefs::get_(&prefs, "hud_background", &_hud_background);
    Prefs::get_(&prefs, "hud_background_color", &_hud_background_color);
}

View_Prefs::~View_Prefs()
{
    //DJV_DEBUG("View_Prefs::~View_Prefs");

    Prefs prefs(Prefs::prefs(), "view");
    Prefs::set_(&prefs, "background", _background);
    Prefs::set_(&prefs, "resize", _resize);
    Prefs::set_(&prefs, "grid", _grid);
    Prefs::set_(&prefs, "grid_color", _grid_color);
    Prefs::set_(&prefs, "hud", _hud);
    Prefs::set_(&prefs, "hud_show", _hud_show);
    Prefs::set_(&prefs, "hud_color", _hud_color);
    Prefs::set_(&prefs, "hud_background", _hud_background);
    Prefs::set_(&prefs, "hud_background_color", _hud_background_color);
}

void View_Prefs::background(const Color & in)
{
    if (in == _background)
    {
        return;
    }

    _background = in;

    DJV_APP->window_update();
}

const Color & View_Prefs::background() const
{
    return _background;
}

void View_Prefs::resize(View::RESIZE in)
{
    if (in == _resize)
    {
        return;
    }

    _resize = in;

    resize_signal.emit(_resize);
}

View::RESIZE View_Prefs::resize() const
{
    return _resize;
}

void View_Prefs::grid(View::GRID in)
{
    if (in == _grid)
    {
        return;
    }

    _grid = in;

    grid_signal.emit(_grid);
}

View::GRID View_Prefs::grid() const
{
    return _grid;
}

void View_Prefs::grid_color(const Color & in)
{
    if (in == _grid_color)
    {
        return;
    }

    _grid_color = in;

    DJV_APP->window_update();
}

const Color & View_Prefs::grid_color() const
{
    return _grid_color;
}

void View_Prefs::hud(bool in)
{
    if (in == _hud)
    {
        return;
    }

    _hud = in;

    hud_signal.emit(_hud);
}

bool View_Prefs::hud() const
{
    return _hud;
}

void View_Prefs::hud_show(View::HUD_SHOW show, bool in)
{
    if (in == _hud_show[show])
    {
        return;
    }

    _hud_show[show] = in;

    DJV_APP->window_update();
}

List<bool> View_Prefs::hud_show() const
{
    return _hud_show;
}

bool View_Prefs::hud_show(View::HUD_SHOW in) const
{
    return _hud_show[in];
}

void View_Prefs::hud_color(const Color & in)
{
    if (in == _hud_color)
    {
        return;
    }

    _hud_color = in;

    DJV_APP->window_update();
}

const Color & View_Prefs::hud_color() const
{
    return _hud_color;
}

void View_Prefs::hud_background(View::HUD_BACKGROUND in)
{
    if (in == _hud_background)
    {
        return;
    }

    _hud_background = in;

    DJV_APP->window_update();
}

View::HUD_BACKGROUND View_Prefs::hud_background() const
{
    return _hud_background;
}

void View_Prefs::hud_background_color(const Color & in)
{
    if (in == _hud_background_color)
    {
        return;
    }

    _hud_background_color = in;

    DJV_APP->window_update();
}

const Color & View_Prefs::hud_background_color() const
{
    return _hud_background_color;
}

//------------------------------------------------------------------------------
// View_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_group = "General",
    label_background = "Background color:",
    label_resize = "Resizing:",
    label_grid_group = "Grid",
    label_grid_color = "Color:",
    label_hud_group = "HUD (Heads Up Display)",
    label_hud = "Enable HUD",
    label_hud_show = "Show:",
    label_hud_color = "Color:",
    label_hud_background = "Background:",
    label_hud_background_color = "Background color:";

} // namespace

View_Prefs_Widget::View_Prefs_Widget() :
    _background_color_widget    (0),
    _grid_color_widget          (0),
    _hud_show_widget            (0),
    _hud_color_widget           (0),
    _hud_background_color_widget(0)
{
    // Create general widgets.

    Group_Box * general_group = new Group_Box(label_group);

    _background_color_widget = new Color_Swatch_Widget(label_background);

    Radio_Button_Group * resize_widget =
        new Radio_Button_Group(label_resize, View::label_resize());

    // Create grid widgets.

    Group_Box * grid_group = new Group_Box(label_grid_group);

    Radio_Button_Group * grid_widget =
        new Radio_Button_Group(View::label_grid());

    _grid_color_widget = new Color_Swatch_Widget(label_grid_color);

    // Create HUD widgets.

    Group_Box * hud_group = new Group_Box(label_hud_group);

    Check_Button * hud_widget = new Check_Button(label_hud);

    _hud_show_widget =
        new Check_Button_Group(label_hud_show, View::label_hud_show());

    _hud_color_widget = new Color_Swatch_Widget(label_hud_color);

    Radio_Button_Group * hud_background_widget = new Radio_Button_Group(
            label_hud_background,
            View::label_hud_background());

    _hud_background_color_widget =
        new Color_Swatch_Widget(label_hud_background_color);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(general_group);
    Horizontal_Layout * layout_h =
        new Horizontal_Layout(general_group->layout());
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(_background_color_widget);
    layout_h->add(resize_widget);

    layout->add(grid_group);
    layout_h = new Horizontal_Layout(grid_group->layout());
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(grid_widget);
    layout_h->add(_grid_color_widget);
    layout_h->add_stretch();

    layout->add(hud_group);
    hud_group->layout()->add(hud_widget);
    layout_h = new Horizontal_Layout(hud_group->layout());
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(_hud_show_widget);
    layout_h->add(_hud_color_widget);
    layout_h->add(hud_background_widget);
    layout_h->add(_hud_background_color_widget);
    layout_h->add_stretch();

    layout->add_stretch();

    // Initialize.

    _background_color_widget->set(View_Prefs::global()->background());

    resize_widget->set(View_Prefs::global()->resize());

    grid_widget->set(View_Prefs::global()->grid());

    _grid_color_widget->set(View_Prefs::global()->grid_color());

    hud_widget->set(View_Prefs::global()->hud());

    List<bool> hud_show;

    for (uint i = 0; i < View::_HUD_SHOW_SIZE; ++i)
    {
        hud_show += View_Prefs::global()->hud_show(
            static_cast<View::HUD_SHOW>(i));
    }

    _hud_show_widget->set(hud_show);

    _hud_color_widget->set(View_Prefs::global()->hud_color());

    hud_background_widget->set(View_Prefs::global()->hud_background());

    _hud_background_color_widget->set(
        View_Prefs::global()->hud_background_color());

    // Callbacks.

    _background_color_widget->signal.set(this, background_callback);

    resize_widget->signal.set(this, resize_callback);

    grid_widget->signal.set(this, grid_callback);

    _grid_color_widget->signal.set(this, grid_callback);

    hud_widget->signal.set(this, hud_callback);

    _hud_show_widget->signal.set(this, hud_show_callback);

    _hud_color_widget->signal.set(this, hud_color_callback);

    hud_background_widget->signal.set(this, hud_background_callback);

    _hud_background_color_widget->signal.set(
        this, hud_background_color_callback);
}

View_Prefs_Widget::~View_Prefs_Widget()
{}

void View_Prefs_Widget::background_callback(const Color & in)
{
    _background_color_widget->set(in);

    View_Prefs::global()->background(in);
}

void View_Prefs_Widget::resize_callback(int in)
{
    View_Prefs::global()->resize(static_cast<View::RESIZE>(in));
}

void View_Prefs_Widget::grid_callback(int in)
{
    View_Prefs::global()->grid(static_cast<View::GRID>(in));
}

void View_Prefs_Widget::grid_callback(const Color & in)
{
    _grid_color_widget->set(in);

    View_Prefs::global()->grid_color(in);
}

void View_Prefs_Widget::hud_callback(bool in)
{
    View_Prefs::global()->hud(in);
}

void View_Prefs_Widget::hud_show_callback(const List<bool> & in)
{
    for (size_t i = 0; i < in.size(); ++i)
    {
        View_Prefs::global()->hud_show(static_cast<View::HUD_SHOW>(i), in[i]);
    }
}

void View_Prefs_Widget::hud_color_callback(const Color & in)
{
    _hud_color_widget->set(in);

    View_Prefs::global()->hud_color(in);
}

void View_Prefs_Widget::hud_background_callback(int in)
{
    View_Prefs::global()->hud_background(static_cast<View::HUD_BACKGROUND>(in));
}

void View_Prefs_Widget::hud_background_color_callback(const Color & in)
{
    _hud_background_color_widget->set(in);

    View_Prefs::global()->hud_background_color(in);
}

} // djv_view

