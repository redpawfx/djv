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

//! \file djv_quicktime_save_widget.cpp

#if defined(DJV_OSX)

//! \todo Force include order on OS X.

#include <sys/types.h>
#include <sys/stat.h>

#endif

#include <djv_quicktime_save_widget.h>

#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_quicktime
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
label_codec_group = "Codec",
label_quality_group = "Quality";

} // namespace

Save_Widget::Save_Widget() :
    _plugin        (0),
    _codec_widget  (0),
    _quality_widget(0)
{
    // Create codec widgets.

    Group_Box * codec_group = new Group_Box(label_codec_group);

    _codec_widget = new Radio_Button_Group(label_codec());

    // Create quality widgets.

    Group_Box * quality_group = new Group_Box(label_quality_group);

    _quality_widget = new Radio_Button_Group(label_quality());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->spacing(Style::global()->spacing_large());

    layout->add(codec_group);
    codec_group->layout()->add(_codec_widget);

    layout->add(quality_group);
    quality_group->layout()->add(_quality_widget);

    layout->add_stretch();

    // Initialize.

    plugin_update();
    widget_update();

    // Callbacks.

    _codec_widget->signal.set(this, codec_callback);
    _quality_widget->signal.set(this, quality_callback);
}

Save_Widget::~Save_Widget()
{}

Plugin * Save_Widget::copy() const
{
    return new Save_Widget;
}

String Save_Widget::name() const
{
    return djv_quicktime::name;
}

void Save_Widget::set(Image_Io_Base * in)
{
    _plugin = in;

    widget_update();
}

void Save_Widget::codec_callback(int in)
{
    if (! _plugin)
    {
        return;
    }

    _options.codec = static_cast<CODEC>(in);

    plugin_update();

    signal.emit(_plugin);
}

void Save_Widget::quality_callback(int in)
{
    if (! _plugin)
    {
        return;
    }

    _options.quality = static_cast<QUALITY>(in);

    plugin_update();

    signal.emit(_plugin);
}

void Save_Widget::plugin_update()
{
    callbacks(false);

    if (_plugin)
    {
        String tmp;
        tmp << _options.codec;
        _plugin->option(_plugin->options()[Save::CODEC_OPTION], &tmp);
        tmp << _options.quality;
        _plugin->option(_plugin->options()[Save::QUALITY_OPTION], &tmp);
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
            tmp = _plugin->option(_plugin->options()[Save::CODEC_OPTION]);
            tmp >> _options.codec;
            tmp = _plugin->option(_plugin->options()[Save::QUALITY_OPTION]);
            tmp >> _options.quality;
        }
    }
    catch (String)
    {}

    _codec_widget->set(_options.codec);
    _quality_widget->set(_options.quality);

    callbacks(true);
}

} // djv_quicktime
