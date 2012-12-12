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

//! \file djv_dpx_load_widget.cpp

#include <djv_dpx_load_widget.h>

#include <djv_float_edit_slider.h>
#include <djv_form_widget.h>
#include <djv_group_box.h>
#include <djv_int_edit_slider.h>
#include <djv_label.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_dpx
{

//------------------------------------------------------------------------------
// djv_image_load_widget
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_load_widget()
{
    return new Load_Widget;
}

}

//------------------------------------------------------------------------------
// Load_Widget
//------------------------------------------------------------------------------

const String
    label_color_profile_group = "Color Profile",
    label_film_print_group = "Film Print",
    label_film_print_black = "Black:",
    label_film_print_white = "White:",
    label_film_print_gamma = "Gamma:",
    label_film_print_soft_clip = "Soft clip:",
    label_convert_group = "Convert";

Load_Widget::Load_Widget() :
    _plugin              (0),
    _color_profile_widget(0),
    _black_widget        (0),
    _white_widget        (0),
    _gamma_widget        (0),
    _soft_clip_widget    (0),
    _convert_widget      (0)
{
    //DJV_DEBUG("Load_Widget::Load_Widget");

    // Create color profile widgets.

    Group_Box * color_profile_group =
        new Group_Box(label_color_profile_group);

    _color_profile_widget =
        new Radio_Button_Group(djv_cineon::label_color_profile());

    Group_Box * film_print_group = new Group_Box(label_film_print_group);

    _black_widget = new Int_Edit_Slider(0, 1023);

    _white_widget = new Int_Edit_Slider(0, 1023);

    _gamma_widget = new Float_Edit_Slider(0.01, 4.0);

    _soft_clip_widget = new Int_Edit_Slider(0, 50);

    // Create convert widgets.

    Group_Box * convert_group = new Group_Box(label_convert_group);

    _convert_widget = new Radio_Button_Group(djv_cineon::label_convert());

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
    form_widget->add_row(label_film_print_soft_clip, _soft_clip_widget);
    
    layout->add(convert_group);
    convert_group->layout()->add(_convert_widget);

    layout->add_stretch();

    // Initialize.

    _black_widget->default_value(Load::Options().film_print.black);
    _white_widget->default_value(Load::Options().film_print.white);
    _gamma_widget->default_value(Load::Options().film_print.gamma);
    _soft_clip_widget->default_value(Load::Options().film_print.soft_clip);

    plugin_update();
    widget_update();

    // Callbacks.

    _color_profile_widget->signal.set(this, color_profile_callback);

    _black_widget->signal.set(this, black_callback);
    _white_widget->signal.set(this, white_callback);
    _gamma_widget->signal.set(this, gamma_callback);
    _soft_clip_widget->signal.set(this, soft_clip_callback);

    _convert_widget->signal.set(this, convert_callback);
}

Load_Widget::~Load_Widget()
{}

Plugin * Load_Widget::copy() const
{
    return new Load_Widget;
}

String Load_Widget::name() const
{
    return djv_dpx::name;
}

void Load_Widget::set(Image_Io_Base * in)
{
    _plugin = in;

    widget_update();
}

void Load_Widget::color_profile_callback(int in)
{
    _options.color_profile = static_cast<djv_cineon::COLOR_PROFILE>(in);

    callback(true);
}

void Load_Widget::black_callback(int in)
{
    _options.film_print.black = in;

    callback(true);
}

void Load_Widget::white_callback(int in)
{
    _options.film_print.white = in;

    callback(true);
}

void Load_Widget::gamma_callback(double in)
{
    _options.film_print.gamma = in;

    callback(true);
}

void Load_Widget::soft_clip_callback(int in)
{
    _options.film_print.soft_clip = in;

    callback(true);
}

void Load_Widget::convert_callback(int in)
{
    _options.convert = static_cast<djv_cineon::CONVERT>(in);

    callback(true);
}

void Load_Widget::callback(bool)
{
    if (! _plugin)
    {
        return;
    }

    plugin_update();

    signal.emit(_plugin);
}

void Load_Widget::plugin_update()
{
    callbacks(false);

    if (_plugin)
    {
        String tmp;
        tmp << _options.color_profile;
        _plugin->option(_plugin->options()[Load::COLOR_PROFILE_OPTION], &tmp);
        tmp << _options.film_print;
        _plugin->option(_plugin->options()[Load::FILM_PRINT_OPTION], &tmp);
        tmp << _options.convert;
        _plugin->option(_plugin->options()[Load::CONVERT_OPTION], &tmp);
    }

    callbacks(true);
}

void Load_Widget::widget_update()
{
    callbacks(false);

    try
    {
        if (_plugin)
        {
            String tmp;
            tmp = _plugin->option(
                _plugin->options()[Load::COLOR_PROFILE_OPTION]);
            tmp >> _options.color_profile;
            tmp = _plugin->option(_plugin->options()[Load::FILM_PRINT_OPTION]);
            tmp >> _options.film_print;
            tmp = _plugin->option(_plugin->options()[Load::CONVERT_OPTION]);
            tmp >> _options.convert;
        }
    }
    catch (String) {}

    _color_profile_widget->set(_options.color_profile);

    _black_widget->set(_options.film_print.black);
    _white_widget->set(_options.film_print.white);
    _gamma_widget->set(_options.film_print.gamma);
    _soft_clip_widget->set(_options.film_print.soft_clip);

    _convert_widget->set(_options.convert);

    callbacks(true);
}

} // djv_dpx

