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

//! \file djv_lut_load.cpp

#include <djv_lut_load_widget.h>

#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_lut
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
    label_format_group = "File Format",
    label_type_group = "File Type";

} // namespace

Load_Widget::Load_Widget() :
    _plugin       (0),
    _format_widget(0),
    _type_widget  (0)
{
    // Create widgets.

    Group_Box * format_group = new Group_Box(label_format_group);

    _format_widget = new Radio_Button_Group(label_format());

    Group_Box * type_group = new Group_Box(label_type_group);

    _type_widget = new Radio_Button_Group(label_type());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(format_group);
    format_group->layout()->add(_format_widget);

    layout->add(type_group);
    type_group->layout()->add(_type_widget);

    layout->add_stretch();

    // Initialize.

    plugin_update();
    widget_update();

    // Callbacks.

    _format_widget->signal.set(this, format_callback);
    _type_widget->signal.set(this, type_callback);
}

Load_Widget::~Load_Widget()
{}

Plugin * Load_Widget::copy() const
{
    return new Load_Widget;
}

String Load_Widget::name() const
{
    return djv_lut::name;
}

void Load_Widget::set(Image_Io_Base * in)
{
    _plugin = in;

    widget_update();
}

void Load_Widget::format_callback(int in)
{
    if (! _plugin)
    {
        return;
    }

    _options.format = static_cast<FORMAT>(in);

    plugin_update();

    signal.emit(_plugin);
}

void Load_Widget::type_callback(int in)
{
    if (! _plugin)
    {
        return;
    }

    _options.type = static_cast<TYPE>(in);

    plugin_update();

    signal.emit(_plugin);
}

void Load_Widget::plugin_update()
{
    callbacks(false);

    if (_plugin)
    {
        String tmp;
        tmp << _options.format;
        _plugin->option(_plugin->options()[Load::FORMAT_OPTION], &tmp);
        tmp << _options.type;
        _plugin->option(_plugin->options()[Load::TYPE_OPTION], &tmp);
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
            tmp = _plugin->option(_plugin->options()[Load::FORMAT_OPTION]);
            tmp >> _options.format;
            tmp = _plugin->option(_plugin->options()[Load::TYPE_OPTION]);
            tmp >> _options.type;
        }
    }
    catch (String) {}

    _format_widget->set(_options.format);
    _type_widget->set(_options.type);

    callbacks(true);
}

} // djv_lut

