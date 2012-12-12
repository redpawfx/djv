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

//! \file djv_radio_button_group.cpp

#include <djv_radio_button_group.h>

#include <djv_radio_button.h>
#include <djv_row_layout.h>
#include <djv_label.h>
#include <djv_style.h>

namespace djv
{

//------------------------------------------------------------------------------
// Radio_Button_Group
//------------------------------------------------------------------------------

void Radio_Button_Group::init()
{
    debug_path("Radio_Button_Group");
    
    _title_widget = new Label;

    _layout = new Vertical_Layout(this);
    _layout->margin(0);
    _layout->add(_title_widget);
    _widget_layout = new Vertical_Layout(_layout);
    _widget_layout->margin(0);

    widget_update();
    value_update();
}

Radio_Button_Group::Radio_Button_Group(const List<String> & label) :
    signal        (this),
    _value        (-1),
    _label        (label),
    _title_widget (0),
    _widget_layout(0),
    _layout       (0)
{
    init();
}

Radio_Button_Group::Radio_Button_Group(
    const String &       title,
    const List<String> & label) :
    signal        (this),
    _value        (-1),
    _title        (title),
    _label        (label),
    _title_widget (0),
    _widget_layout(0),
    _layout       (0)
{
    init();
}

Radio_Button_Group::~Radio_Button_Group()
{}

void Radio_Button_Group::set(int in)
{
    //DJV_DEBUG("Radio_Button_Group::set");

    const int size = static_cast<int>(_widgets.size());

    in = Math::clamp(in, 0, size - 1);

    //DJV_DEBUG_PRINT("in = " << in);

    if (in == _value)
    {
        return;
    }

    _value = in;

    value_update();

    signal.emit(_value);
}

int Radio_Button_Group::get() const
{
    return _value;
}

void Radio_Button_Group::title(const String & in)
{
    if (in == _title)
    {
        return;
    }

    _title = in;

    widget_update();
}

const String & Radio_Button_Group::title() const
{
    return _title;
}

void Radio_Button_Group::label(const List<String> & in)
{
    if (in == _label)
    {
        return;
    }

    _label = in;

    widget_update();
}

const List<String> & Radio_Button_Group::label() const
{
    return _label;
}

void Radio_Button_Group::widget_callback(int in)
{
    //DJV_DEBUG("Radio_Button_Group::widget_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    set(in);
}

void Radio_Button_Group::value_update()
{
    //DJV_DEBUG("Radio_Button_Group::value_update");

    callbacks(false);

    if (_value >= 0 && _value < static_cast<int>(_widgets.size()))
    {
        _widgets[_value]->set(true);
    }

    callbacks(true);
}

void Radio_Button_Group::widget_update()
{
    //DJV_DEBUG("Radio_Button_Group::widget_update");

    callbacks(false);

    // Create widgets.

    _widget_layout->del();

    _widgets.resize(_label.size());

    for (size_t i = 0; i < _widgets.size(); ++i)
    {
        _widgets[i] = new Radio_Button(_label[i]);
    }

    Radio_Button::create_group(_widgets);

    // Layout.

    for (size_t i = 0; i < _widgets.size(); ++i)
    {
        _widget_layout->add(_widgets[i]);
    }

    // Initialize.

    if (! _title.empty())
    {
        _title_widget->set(_title);
        _title_widget->show();
    }
    else
    {
        _title_widget->hide();
    }

    update();

    // Callbacks.

    for (size_t i = 0; i < _widgets.size(); ++i)
    {
        _widgets[i]->group_signal.set(this, widget_callback);
    }

    callbacks(true);
}

} // djv

