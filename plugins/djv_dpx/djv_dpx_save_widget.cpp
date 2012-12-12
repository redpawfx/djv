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

//! \file djv_dpx_save_widget.cpp

#include <djv_dpx_save_widget.h>

#include <djv_form_widget.h>
#include <djv_float_edit_slider.h>
#include <djv_group_box.h>
#include <djv_int_edit_slider.h>
#include <djv_label.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_dpx
{

//------------------------------------------------------------------------------
// djv_image_save_widget
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_save_widget()
{
    return new Save_Widget;
}

}

//------------------------------------------------------------------------------
// Save_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_color_profile_group = "Color Profile",
    label_film_print_group = "Film Print",
    label_film_print_black = "Black:",
    label_film_print_white = "White:",
    label_film_print_gamma = "Gamma:",
    label_version_group = "File Version",
    label_type_group = "File Type",
    label_endian_group = "File Endian";

} // namespace

Save_Widget::Save_Widget() :
    _plugin              (0),
    _color_profile_widget(0),
    _black_widget        (0),
    _white_widget        (0),
    _gamma_widget        (0),
    _version_widget      (0),
    _type_widget         (0),
    _endian_widget       (0)
{
    // Create color profile widgets.

    Group_Box * color_profile_group =
        new Group_Box(label_color_profile_group);

    _color_profile_widget =
        new Radio_Button_Group(djv_cineon::label_color_profile());

    Group_Box * film_print_group = new Group_Box(label_film_print_group);

    _black_widget = new Int_Edit_Slider(0, 1023);

    _white_widget = new Int_Edit_Slider(0, 1023);

    _gamma_widget = new Float_Edit_Slider(0.01, 4.0);

    // Create version widgets.

    Group_Box * version_group =
        new Group_Box(label_version_group);

    _version_widget = new Radio_Button_Group(label_version());

    // Create type widgets.

    Group_Box * type_group =
        new Group_Box(label_type_group);

    _type_widget = new Radio_Button_Group(label_type());

    // Create endian widgets.

    Group_Box * endian_group =
        new Group_Box(label_endian_group);

    _endian_widget = new Radio_Button_Group(label_endian());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(color_profile_group);
    color_profile_group->layout()->add(_color_profile_widget);

    color_profile_group->layout()->add(film_print_group);
    Form_Widget * form_widget = new Form_Widget;
    film_print_group->layout()->add(form_widget);
    form_widget->add_row(label_film_print_black, _black_widget);
    form_widget->add_row(label_film_print_white, _white_widget);
    form_widget->add_row(label_film_print_gamma, _gamma_widget);
    
    layout->add(version_group);
    version_group->layout()->add(_version_widget);

    layout->add(type_group);
    type_group->layout()->add(_type_widget);

    layout->add(endian_group);
    endian_group->layout()->add(_endian_widget);

    layout->add_stretch();

    // Initialize.

    _black_widget->default_value(Save::Options().film_print.black);
    _white_widget->default_value(Save::Options().film_print.white);
    _gamma_widget->default_value(Save::Options().film_print.gamma);

    plugin_update();
    widget_update();

    // Callbacks.

    _color_profile_widget->signal.set(this, color_profile_callback);
    _black_widget->signal.set(this, black_callback);
    _white_widget->signal.set(this, white_callback);
    _gamma_widget->signal.set(this, gamma_callback);
    _version_widget->signal.set(this, version_callback);
    _type_widget->signal.set(this, type_callback);
    _endian_widget->signal.set(this, endian_callback);
}

Save_Widget::~Save_Widget()
{}

Plugin * Save_Widget::copy() const
{
    return new Save_Widget;
}

String Save_Widget::name() const
{
    return djv_dpx::name;
}

void Save_Widget::set(Image_Io_Base * in)
{
    _plugin = in;

    widget_update();
}

void Save_Widget::color_profile_callback(int in)
{
    _options.color_profile = static_cast<djv_cineon::COLOR_PROFILE>(in);

    callback(true);
}

void Save_Widget::black_callback(int in)
{
    _options.film_print.black = in;

    callback(true);
}

void Save_Widget::white_callback(int in)
{
    _options.film_print.white = in;

    callback(true);
}

void Save_Widget::gamma_callback(double in)
{
    _options.film_print.gamma = in;

    callback(true);
}

void Save_Widget::version_callback(int in)
{
    _options.version = static_cast<VERSION>(in);

    callback(true);
}

void Save_Widget::type_callback(int in)
{
    _options.type = static_cast<TYPE>(in);

    callback(true);
}

void Save_Widget::endian_callback(int in)
{
    _options.endian = static_cast<ENDIAN>(in);

    callback(true);
}

void Save_Widget::callback(bool)
{
    if (! _plugin)
    {
        return;
    }

    plugin_update();

    signal.emit(_plugin);
}

void Save_Widget::plugin_update()
{
    callbacks(false);

    if (_plugin)
    {
        String tmp;
        tmp << _options.color_profile;
        _plugin->option(_plugin->options()[Save::COLOR_PROFILE_OPTION], &tmp);
        tmp << _options.film_print;
        _plugin->option(_plugin->options()[Save::FILM_PRINT_OPTION], &tmp);
        tmp << _options.version;
        _plugin->option(_plugin->options()[Save::VERSION_OPTION], &tmp);
        tmp << _options.type;
        _plugin->option(_plugin->options()[Save::TYPE_OPTION], &tmp);
        tmp << _options.endian;
        _plugin->option(_plugin->options()[Save::ENDIAN_OPTION], &tmp);
    }

    callbacks(true);
}

void Save_Widget::widget_update()
{
    callbacks(false);

    try
    {
        if (_plugin)
        {
            String tmp;
            tmp = _plugin->option(
                _plugin->options()[Save::COLOR_PROFILE_OPTION]);
            tmp >> _options.color_profile;
            tmp = _plugin->option(_plugin->options()[Save::FILM_PRINT_OPTION]);
            tmp >> _options.film_print;
            tmp = _plugin->option(_plugin->options()[Save::VERSION_OPTION]);
            tmp >> _options.version;
            tmp = _plugin->option(_plugin->options()[Save::TYPE_OPTION]);
            tmp >> _options.type;
            tmp = _plugin->option(_plugin->options()[Save::ENDIAN_OPTION]);
            tmp >> _options.endian;
        }
    }
    catch (String) {}

    _color_profile_widget->set(_options.color_profile);
    _black_widget->set(_options.film_print.black);
    _white_widget->set(_options.film_print.white);
    _gamma_widget->set(_options.film_print.gamma);
    _version_widget->set(_options.version);
    _type_widget->set(_options.type);
    _endian_widget->set(_options.endian);

    callbacks(true);
}

} // djv_dpx

