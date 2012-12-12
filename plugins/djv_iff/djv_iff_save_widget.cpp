//------------------------------------------------------------------------------
// Copyright (c) 2008-2009 Mikael Sundell
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

//! \file djv_iff_save_widget.cpp

#include <djv_iff_save_widget.h>

#include <djv_group_box.h>
#include <djv_label.h>
#include <djv_radio_button_group.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_iff
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
    label_compression_group = "Compression";

} // namespace

Save_Widget::Save_Widget() :
    _plugin            (0),
    _compression_widget(0)
{
    //DJV_DEBUG("Save_Widget::Save_Widget");

    // Create widgets.

    Group_Box * group = new Group_Box(label_compression_group);

    _compression_widget = new Radio_Button_Group(label_compression());

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(group);
    group->layout()->add(_compression_widget);

    layout->add_stretch();

    // Initialize.

    plugin_update();
    widget_update();

    // Callbacks.

    _compression_widget->signal.set(this, compression_callback);
}

Save_Widget::~Save_Widget()
{}

Plugin * Save_Widget::copy() const
{
    return new Save_Widget;
}

String Save_Widget::name() const
{
    return djv_iff::name;
}

void Save_Widget::set(Image_Io_Base * in)
{
    _plugin = in;

    widget_update();
}

void Save_Widget::compression_callback(int in)
{
    if (! _plugin)
    {
        return;
    }

    _options.compression = static_cast<COMPRESSION>(in);

    plugin_update();

    signal.emit(_plugin);
}

void Save_Widget::plugin_update()
{
    callbacks(false);

    if (_plugin)
    {
        String tmp;
        tmp << _options.compression;
        _plugin->option(_plugin->options()[Save::COMPRESSION_OPTION], &tmp);
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
            tmp = _plugin->option(_plugin->options()[Save::COMPRESSION_OPTION]);
            tmp >> _options.compression;
        }
    }
    catch (String) {}

    _compression_widget->set(_options.compression);

    callbacks(true);
}

} // djv_iff

