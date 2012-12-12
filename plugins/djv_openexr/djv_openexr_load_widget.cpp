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

//! \file djv_openexr_load_widget.cpp

#include <djv_openexr_load_widget.h>

#include <djv_form_widget.h>
#include <djv_float_edit_slider.h>
#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_openexr
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

namespace
{

const String
    label_color_profile_group = "Color Profile",
    label_gamma_group = "Gamma",
    label_gamma = "Value:",
    label_exposure_group = "Exposure",
    label_exposure = "Value:",
    label_exposure_defog = "Defog:",
    label_exposure_knee_low = "Knee low:",
    label_exposure_knee_high = "Knee high:",
    label_channels_group = "Channels";

} // namespace

Load_Widget::Load_Widget() :
    _plugin                   (0),
    _color_profile_widget     (0),
    _gamma_widget             (0),
    _exposure_widget          (0),
    _exposure_defog_widget    (0),
    _exposure_knee_low_widget (0),
    _exposure_knee_high_widget(0),
    _channels_widget          (0)
{
    //DJV_DEBUG("Load_Widget::Load_Widget");

    // Create color profile widgets.

    Group_Box * color_profile_group =
        new Group_Box(label_color_profile_group);

    _color_profile_widget = new Radio_Button_Group(label_color_profile());

    Group_Box * gamma_group = new Group_Box(label_gamma_group);

    _gamma_widget = new Float_Edit_Slider(0.1, 4.0, label_gamma);

    Group_Box * exposure_group = new Group_Box(label_exposure_group);

    _exposure_widget = new Float_Edit_Slider(-10.0, 10.0);

    _exposure_defog_widget = new Float_Edit_Slider(0.0, 0.01);

    _exposure_knee_low_widget = new Float_Edit_Slider(-3.0, 3.0);

    _exposure_knee_high_widget = new Float_Edit_Slider(3.5, 7.5);

    // Create group channel widgets.

    Group_Box * channels_group =
        new Group_Box(label_channels_group);

    _channels_widget = new Radio_Button_Group(label_channels());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(color_profile_group);
    color_profile_group->layout()->add(_color_profile_widget);
    color_profile_group->layout()->add(gamma_group);
    Vertical_Layout * layout_v = new Vertical_Layout(gamma_group->layout());
    layout_v->margin(0);
    layout_v->add(_gamma_widget);
    color_profile_group->layout()->add(exposure_group);
    Form_Widget * form_widget = new Form_Widget;
    exposure_group->layout()->add(form_widget);
    form_widget->add_row(label_exposure, _exposure_widget);
    form_widget->add_row(label_exposure_defog, _exposure_defog_widget);
    form_widget->add_row(label_exposure_knee_low, _exposure_knee_low_widget);
    form_widget->add_row(label_exposure_knee_high, _exposure_knee_high_widget);

    layout->add(channels_group);
    layout_v = new Vertical_Layout(channels_group->layout());
    layout_v->margin(0);
    layout_v->add(_channels_widget);

    layout->add_stretch();

    // Initialize.

    _exposure_widget->inc(0.1, 1.0);
    _exposure_defog_widget->inc(0.0001, 0.001);
    _exposure_knee_low_widget->inc(0.1, 1.0);
    _exposure_knee_high_widget->inc(0.1, 1.0);

    _gamma_widget->default_value(Load::Options().gamma);
    _exposure_widget->default_value(Load::Options().exposure.value);
    _exposure_defog_widget->default_value(Load::Options().exposure.defog);
    _exposure_knee_low_widget->default_value(Load::Options().exposure.knee_low);
    _exposure_knee_high_widget->default_value(
        Load::Options().exposure.knee_high);

    plugin_update();
    widget_update();

    // Callbacks.

    _color_profile_widget->signal.set(this, color_profile_callback);
    _gamma_widget->signal.set(this, gamma_callback);
    _exposure_widget->signal.set(this, exposure_callback);
    _exposure_defog_widget->signal.set(this, exposure_defog_callback);
    _exposure_knee_low_widget->signal.set(this, exposure_knee_low_callback);
    _exposure_knee_high_widget->signal.set(this, exposure_knee_high_callback);
    _channels_widget->signal.set(this, channels_callback);
}

Load_Widget::~Load_Widget()
{}

Plugin * Load_Widget::copy() const
{
    return new Load_Widget;
}

String Load_Widget::name() const
{
    return djv_openexr::name;
}

void Load_Widget::set(Image_Io_Base * in)
{
    _plugin = in;

    widget_update();
}

void Load_Widget::color_profile_callback(int in)
{
    _options.color_profile = static_cast<COLOR_PROFILE>(in);

    callback(true);
}

void Load_Widget::gamma_callback(double in)
{
    _options.gamma = in;

    callback(true);
}

void Load_Widget::exposure_callback(double in)
{
    _options.exposure.value = in;

    callback(true);
}

void Load_Widget::exposure_defog_callback(double in)
{
    _options.exposure.defog = in;

    callback(true);
}

void Load_Widget::exposure_knee_low_callback(double in)
{
    _options.exposure.knee_low = in;

    callback(true);
}

void Load_Widget::exposure_knee_high_callback(double in)
{
    _options.exposure.knee_high = in;

    callback(true);
}

void Load_Widget::channels_callback(int in)
{
    _options.channels = static_cast<CHANNELS>(in);

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
    //DJV_DEBUG("Load_Widget::plugin_update()");
    //DJV_DEBUG_PRINT("gamma = " << _options.gamma);

    callbacks(false);

    if (_plugin)
    {
        const List<String> & list = _plugin->options();

        String tmp;
        tmp << _options.color_profile;
        _plugin->option(list[Load::COLOR_PROFILE_OPTION], &tmp);
        tmp << _options.gamma;
        _plugin->option(list[Load::GAMMA_OPTION], &tmp);
        tmp << _options.exposure;
        _plugin->option(list[Load::EXPOSURE_OPTION], &tmp);
        tmp << _options.channels;
        _plugin->option(list[Load::CHANNELS_OPTION], &tmp);
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
            const List<String> & list = _plugin->options();

            String tmp;
            tmp = _plugin->option(list[Load::COLOR_PROFILE_OPTION]);
            tmp >> _options.color_profile;
            tmp = _plugin->option(list[Load::GAMMA_OPTION]);
            tmp >> _options.gamma;
            tmp = _plugin->option(list[Load::EXPOSURE_OPTION]);
            tmp >> _options.exposure;
            tmp = _plugin->option(list[Load::CHANNELS_OPTION]);
            tmp >> _options.channels;
        }
    }
    catch (String)
    {}

    _color_profile_widget->set(_options.color_profile);
    _gamma_widget->set(_options.gamma);
    _exposure_widget->set(_options.exposure.value);
    _exposure_defog_widget->set(_options.exposure.defog);
    _exposure_knee_low_widget->set(_options.exposure.knee_low);
    _exposure_knee_high_widget->set(_options.exposure.knee_high);
    _channels_widget->set(_options.channels);

    callbacks(true);
}

} // djv_openexr

