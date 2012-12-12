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

//! \file djv_openexr_base_widget.cpp

#include <djv_openexr_base_widget.h>

#include <djv_group_box.h>
#include <djv_int_edit.h>
#include <djv_multiline_label.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv_openexr
{

//------------------------------------------------------------------------------
// djv_image_io_base_widget
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_io_base_widget()
{
    return new Base_Widget;
}

}

//------------------------------------------------------------------------------
// Base_Widget
//------------------------------------------------------------------------------

namespace
{

const String
    label_threads_group = "Threads",
    label_threads_text =
        "Set the maximum number of threads to use. A value of zero disables "
        "multi-threading.";

} // namespace

Base_Widget::Base_Widget() :
    _plugin        (0),
    _threads_widget(0)
{
    //DJV_DEBUG("Base_Widget::Base_Widget");

    // Create thread widgets.

    Group_Box * threads_group = new Group_Box(label_threads_group);

    _threads_widget = new Int_Edit;
    _threads_widget->range(0, 4096);

    Multiline_Label * threads_text = new Multiline_Label(label_threads_text);

    // Layout.

    Vertical_Layout * layout = new Vertical_Layout(this);
    layout->margin(Style::global()->margin_large());
    layout->spacing(Style::global()->spacing_large());

    layout->add(threads_group);
    threads_group->layout()->add(threads_text);
    threads_group->layout()->add(_threads_widget);

    layout->add_stretch();

    // Initialize.

    plugin_update();
    widget_update();

    // Callbacks.

    _threads_widget->signal.set(this, threads_callback);
}

Base_Widget::~Base_Widget()
{}

Plugin * Base_Widget::copy() const
{
    return new Base_Widget;
}

String Base_Widget::name() const
{
    return djv_openexr::name;
}

void Base_Widget::set(Image_Io_Base * in)
{
    _plugin = in;

    widget_update();
}

void Base_Widget::threads_callback(int in)
{
    _options.threads = in;

    callback(true);
}

void Base_Widget::callback(bool)
{
    if (! _plugin)
    {
        return;
    }

    plugin_update();

    signal.emit(_plugin);
}

void Base_Widget::plugin_update()
{
    callbacks(false);

    if (_plugin)
    {
        const List<String> & list = _plugin->options();

        String tmp;
        tmp << _options.threads;
        _plugin->option(list[Base::THREADS], &tmp);
    }

    callbacks(true);
}

void Base_Widget::widget_update()
{
    callbacks(false);

    try
    {
        if (_plugin)
        {
            const List<String> & list = _plugin->options();

            String tmp;
            tmp = _plugin->option(list[Base::THREADS]);
            tmp >> _options.threads;
        }
    }
    catch (String)
    {}

    _threads_widget->set(_options.threads);

    callbacks(true);
}

} // djv_openexr

