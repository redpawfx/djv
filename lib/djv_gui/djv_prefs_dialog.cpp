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

//! \file djv_prefs_dialog.cpp

#include <djv_prefs_dialog.h>

#include <djv_check_button.h>
#include <djv_choice_widget.h>
#include <djv_color_swatch.h>
#include <djv_image_prefs.h>
#include <djv_int_edit.h>
#include <djv_file_browser_prefs.h>
#include <djv_float_edit.h>
#include <djv_group_box.h>
#include <djv_image.h>
#include <djv_image_io_widget.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_push_button.h>
#include <djv_radio_button_group.h>
#include <djv_shortcut.h>
#include <djv_style.h>
#include <djv_tabs.h>
#include <djv_tooltip_prefs.h>

namespace djv
{

//------------------------------------------------------------------------------
// Style_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_color_group = "Color",
    label_color_foreground = "Foreground:",
    label_color_background = "Background:",
    label_color_background2 = "Background 2:",
    label_color_button = "Button:",
    label_color_select = "Select:",
    label_color_highlight = "Highlight:",
    label_focus = "Draw keyboard focus",
    label_highlight = "Draw highlight",
    label_size_group = "Size",
    label_size_value = "Value:";

} // namespace

Style_Prefs_Widget::Style_Prefs_Widget() :
    _color_widget            (0),
    _color_foreground_widget (0),
    _color_background_widget (0),
    _color_background2_widget(0),
    _color_button_widget     (0),
    _color_select_widget     (0),
    _color_highlight_widget  (0),
    _focus_widget            (0),
    _highlight_widget        (0),
    _size_widget             (0),
    _size_value_widget       (0)
{
    // Create widgets.

    Group_Box * color_group = new Group_Box(label_color_group);

    _color_widget = new Radio_Button_Group(Style::label_color());

    _color_foreground_widget = new Color_Swatch_Widget(label_color_foreground);

    _color_background_widget = new Color_Swatch_Widget(label_color_background);

    _color_background2_widget =
        new Color_Swatch_Widget(label_color_background2);

    _color_button_widget = new Color_Swatch_Widget(label_color_button);

    _color_select_widget = new Color_Swatch_Widget(label_color_select);

    _color_highlight_widget = new Color_Swatch_Widget(label_color_highlight);

    _focus_widget = new Check_Button(label_focus);

    _highlight_widget = new Check_Button(label_highlight);

    Group_Box * size_group = new Group_Box(label_size_group);

    _size_widget = new Radio_Button_Group(Style::label_size());

    _size_value_widget = new Int_Edit;

    Label * size_value_label = new Label(label_size_value);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(color_group);
    color_group->layout()->add(_color_widget);
    Vertical_Layout * layout_v = new Vertical_Layout(color_group->layout());
    layout_v->margin(0);
    Horizontal_Layout * layout_h = new Horizontal_Layout(color_group->layout());
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(_color_foreground_widget);
    layout_h->add(_color_background_widget);
    layout_h->add(_color_background2_widget);
    layout_h->add(_color_button_widget);
    layout_h->add(_color_select_widget);
    layout_h->add(_color_highlight_widget);
    color_group->layout()->add(_focus_widget);
    color_group->layout()->add(_highlight_widget);

    layout->add(size_group);
    size_group->layout()->add(_size_widget);
    layout_h = new Horizontal_Layout(size_group->layout());
    layout_h->margin(0);
    layout_h->add(size_value_label);
    layout_h->add(_size_value_widget);

    layout->add_stretch();

    // Initialize.

    color_update();
    size_update();

    // Callbacks.

    _color_widget->signal.set(this, color_callback);
    _color_foreground_widget->signal.set(this, color_foreground_callback);
    _color_background_widget->signal.set(this, color_background_callback);
    _color_background2_widget->signal.set(this, color_background2_callback);
    _color_button_widget->signal.set(this, color_button_callback);
    _color_select_widget->signal.set(this, color_select_callback);
    _color_highlight_widget->signal.set(this, color_highlight_callback);
    _focus_widget->signal.set(this, focus_callback);
    _highlight_widget->signal.set(this, highlight_callback);

    _size_widget->signal.set(this, size_callback);
    _size_value_widget->signal.set(this, size_value_callback);
}

void Style_Prefs_Widget::color_callback(int in)
{
    Style_Prefs::global()->color(static_cast<Style::COLOR>(in));

    color_update();
}

void Style_Prefs_Widget::color_foreground_callback(const Color & in)
{
    Style::Color tmp = Style_Prefs::global()->color_value();
    tmp.foreground = in;
    Style_Prefs::global()->color_value(tmp);
}

void Style_Prefs_Widget::color_background_callback(const Color & in)
{
    Style::Color tmp = Style_Prefs::global()->color_value();
    tmp.background = in;
    Style_Prefs::global()->color_value(tmp);
}

void Style_Prefs_Widget::color_background2_callback(const Color & in)
{
    Style::Color tmp = Style_Prefs::global()->color_value();
    tmp.background2 = in;
    Style_Prefs::global()->color_value(tmp);
}

void Style_Prefs_Widget::color_button_callback(const Color & in)
{
    Style::Color tmp = Style_Prefs::global()->color_value();
    tmp.button = in;
    Style_Prefs::global()->color_value(tmp);
}

void Style_Prefs_Widget::color_select_callback(const Color & in)
{
    Style::Color tmp = Style_Prefs::global()->color_value();
    tmp.select = in;
    Style_Prefs::global()->color_value(tmp);
}

void Style_Prefs_Widget::color_highlight_callback(const Color & in)
{
    Style::Color tmp = Style_Prefs::global()->color_value();
    tmp.highlight = in;
    Style_Prefs::global()->color_value(tmp);
}

void Style_Prefs_Widget::focus_callback(bool in)
{
    Style_Prefs::global()->focus(in);
}

void Style_Prefs_Widget::highlight_callback(bool in)
{
    Style_Prefs::global()->highlight(in);
}

void Style_Prefs_Widget::size_callback(int in)
{
    Style_Prefs::global()->size(static_cast<Style::SIZE>(in));

    size_update();
}

void Style_Prefs_Widget::size_value_callback(int in)
{
    Style_Prefs::global()->size_value(in);
}

void Style_Prefs_Widget::color_update()
{
    callbacks(false);

    _color_widget->set(Style_Prefs::global()->color());

    _color_foreground_widget->set(
        Style_Prefs::global()->color_value().foreground);
    _color_background_widget->set(
        Style_Prefs::global()->color_value().background);
    _color_background2_widget->set(
        Style_Prefs::global()->color_value().background2);
    _color_button_widget->set(
        Style_Prefs::global()->color_value().button);
    _color_select_widget->set(
        Style_Prefs::global()->color_value().select);
    _color_highlight_widget->set(
        Style_Prefs::global()->color_value().highlight);

    _focus_widget->set(Style_Prefs::global()->focus());
    _highlight_widget->set(Style_Prefs::global()->highlight());

    callbacks(true);
}

void Style_Prefs_Widget::size_update()
{
    callbacks(false);

    _size_widget->set(Style_Prefs::global()->size());
    _size_value_widget->set(Style_Prefs::global()->size_value());

    callbacks(true);
}

//------------------------------------------------------------------------------
// General_Prefs_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_time_group = "Time",
    label_time_units = "Units:",
    label_speed = "Default speed (frames per second):",
    label_filter_group = "Render Filter",
    label_filter_text = "Set the rendering filter quality.\n",
    label_filter_min = "Minify:",
    label_filter_mag = "Magnify:",
    label_help_group = "Help",
    label_help_tooltips = "Tooltips";

} // namespace

General_Prefs_Widget::General_Prefs_Widget()
{
    // Create time widgets.

    Group_Box * time_group = new Group_Box(label_time_group);

    Radio_Button_Group * time_units =
        new Radio_Button_Group(label_time_units, Time::label_units());

    Choice_Widget * speed_widget = new Choice_Widget(Speed::label_fps());
    
    Label * speed_label = new Label(label_speed);

    // Create filter widgets.

    Group_Box * filter_group = new Group_Box(label_filter_group);

    Label * filter_text = new Label(label_filter_text);

    Radio_Button_Group * filter_min =
        new Radio_Button_Group(
        label_filter_min, Gl_Image_Filter::label_filter());

    Radio_Button_Group * filter_mag =
        new Radio_Button_Group(
        label_filter_mag, Gl_Image_Filter::label_filter());

    // Create help widgets.

    Group_Box * help_group =
        new Group_Box(label_help_group);

    Check_Button * help_tooltips =
        new Check_Button(label_help_tooltips);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(time_group);
    time_group->layout()->add(time_units);
    Horizontal_Layout * layout_h = new Horizontal_Layout(time_group->layout());
    layout_h->margin(0);
    layout_h->add(speed_label);
    layout_h->add(speed_widget);

    layout->add(filter_group);
    filter_group->layout()->add(filter_text);
    layout_h = new Horizontal_Layout(filter_group->layout());
    layout_h->spacing(Style::global()->spacing_large());
    layout_h->margin(0);
    layout_h->add(filter_min);
    layout_h->add(filter_mag);
    layout_h->add_stretch();

    layout->add(help_group);
    help_group->layout()->add(help_tooltips);

    layout->add_stretch();

    // Initialize.

    time_units->set(Image_Prefs::global()->units());
    speed_widget->set(Speed::default_fps);

    filter_min->set(Image_Prefs::global()->filter().min);
    filter_mag->set(Image_Prefs::global()->filter().mag);

    help_tooltips->set(Tooltip_Prefs::global()->tooltips());

    // Callbacks.

    time_units->signal.set(this, time_units_callback);
    speed_widget->signal.set(this, speed_callback);

    filter_min->signal.set(this, filter_min_callback);
    filter_mag->signal.set(this, filter_mag_callback);

    help_tooltips->signal.set(this, help_tooltips_callback);
}

void General_Prefs_Widget::time_units_callback(int in)
{
    Image_Prefs::global()->units(static_cast<Time::UNITS>(in));
}

void General_Prefs_Widget::speed_callback(int in)
{
    Speed::default_fps = static_cast<Speed::FPS>(in);
}

void General_Prefs_Widget::filter_min_callback(int in)
{
    Image_Prefs::global()->filter(
        Gl_Image_Filter(
            static_cast<Gl_Image_Filter::FILTER>(in),
            Image_Prefs::global()->filter().mag));
}

void General_Prefs_Widget::filter_mag_callback(int in)
{
    Image_Prefs::global()->filter(
        Gl_Image_Filter(
            Image_Prefs::global()->filter().min,
            static_cast<Gl_Image_Filter::FILTER>(in)));
}

void General_Prefs_Widget::help_tooltips_callback(bool in)
{
    Tooltip_Prefs::global()->tooltips(in);
}

//------------------------------------------------------------------------------
// Prefs_Dialog
//------------------------------------------------------------------------------

namespace
{

const String
    label_close  = "Clos&e";

} // namespace

Prefs_Dialog::Prefs_Dialog() :
    Dialog("Preferences"),
    _tabs         (0),
    _shortcut_tabs(0),
    _close_widget (0)
{
    // Create widgets.

    _tabs = new Tabs;
    _shortcut_tabs = new Tabs;

    _close_widget = new Push_Button(label_close);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->add(_tabs);
    layout->stretch(_tabs);

    Horizontal_Layout * layout_h = new Horizontal_Layout(layout);
    layout_h->margin(0);
    layout_h->add_stretch();
    layout_h->add(_close_widget);
    layout_h->add_spacer(Layout::window_handle_size());

    // Initialize.

    add("General", new General_Prefs_Widget);
    add("Style", new Style_Prefs_Widget);

    add("Image I/O", new Image_Io_Prefs_Widget);

    add("Shortcuts", _shortcut_tabs);
    add_shortcut("File Browser", new File_Browser_Prefs_Widget);

    size(V2i(600, 600));

    // Callbacks.

    _close_widget->signal.set(this, close_callback);
}

Prefs_Dialog::~Prefs_Dialog()
{}

void Prefs_Dialog::add(const String & name, Abstract_Widget * in)
{
    //DJV_DEBUG("Prefs_Dialog::add");
    //DJV_DEBUG_PRINT("name = " << name);

    _tabs->tab_insert(name, in);
}

void Prefs_Dialog::add_shortcut(const String & name, Abstract_Widget * in)
{
    //DJV_DEBUG("Prefs_Dialog::add_shortcut");
    //DJV_DEBUG_PRINT("name = " << name);

    _shortcut_tabs->tab_insert(name, in);
}

void Prefs_Dialog::show()
{
    _close_widget->take_focus();

    Dialog::show();
}

void Prefs_Dialog::dirty()
{
    Dialog::dirty();
}

Prefs_Dialog * Prefs_Dialog::global()
{
    static Prefs_Dialog * global = 0;

    if (! global)
    {
        global = new Prefs_Dialog;
    }

    return global;
}

void Prefs_Dialog::close_callback(bool)
{
    hide();
}

} // djv

