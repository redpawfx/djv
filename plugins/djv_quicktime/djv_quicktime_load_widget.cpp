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

//! \file djv_quicktime_load_widget.cpp

#if defined(DJV_OSX)

//! \todo Force include order on OS X.

#include <sys/types.h>
#include <sys/stat.h>

#endif

#include <djv_quicktime_load_widget.h>

#include <djv_group_box.h>
#include <djv_int_edit.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_quicktime
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
label_start_frame_group = "Start Frame";

} // namespace

Load_Widget::Load_Widget() :
    _plugin            (0),
    _start_frame_widget(0)
{
    // Create widgets.

    Group_Box * start_frame_group = new Group_Box(label_start_frame_group);

    _start_frame_widget = new Int_Edit;
    _start_frame_widget->range(0, Int_Edit::int_max);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->spacing(Style::global()->spacing_large());

    layout->add(start_frame_group);
    start_frame_group->layout()->add(_start_frame_widget);

    layout->add_stretch();

    // Initialize.

    plugin_update();
    widget_update();

    // Callbacks.

    _start_frame_widget->signal.set(this, start_frame_callback);
}

Load_Widget::~Load_Widget()
{}

Plugin * Load_Widget::copy() const
{
    return new Load_Widget;
}

String Load_Widget::name() const
{
    return djv_quicktime::name;
}

void Load_Widget::set(Image_Io_Base * in)
{
    _plugin = in;

    widget_update();
}

void Load_Widget::start_frame_callback(int in)
{
    if (! _plugin)
    {
        return;
    }

    _options.start_frame = in;

    plugin_update();

    signal.emit(_plugin);
}

void Load_Widget::plugin_update()
{
    callbacks(false);

    if (_plugin)
    {
        String tmp;
        tmp << _options.start_frame;
        _plugin->option(_plugin->options()[Load::START_FRAME_OPTION], &tmp);
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
            tmp = _plugin->option(_plugin->options()[Load::START_FRAME_OPTION]);
            tmp >> _options.start_frame;
        }
    }
    catch (String)
    {}

    _start_frame_widget->set(_options.start_frame);

    callbacks(true);
}

} // djv_quicktime

