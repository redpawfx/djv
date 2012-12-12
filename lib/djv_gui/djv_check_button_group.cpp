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

//! \file djv_check_button_group.cpp

#include <djv_check_button_group.h>

#include <djv_check_button.h>
#include <djv_row_layout.h>
#include <djv_label.h>
#include <djv_style.h>

namespace djv
{

//------------------------------------------------------------------------------
// Check_Button_Group
//------------------------------------------------------------------------------

void Check_Button_Group::init()
{
    debug_path("Check_Button_Group");

    _title_widget = new Label;

    _layout = new Vertical_Layout(this);
    _layout->margin(0);
    _layout->add(_title_widget);
    _widget_layout = new Vertical_Layout(_layout);
    _widget_layout->margin(0);

    widget_update();
    value_update();
}

Check_Button_Group::Check_Button_Group(
    const List<String> & label) :
    signal        (this),
    index_signal  (this),
    _label        (label),
    _title_widget (0),
    _widget_layout(0),
    _layout       (0)
{
    init();
}

Check_Button_Group::Check_Button_Group(
    const String & title,
    const List<String> & label) :
    signal        (this),
    index_signal  (this),
    _title        (title),
    _label        (label),
    _title_widget (0),
    _widget_layout(0),
    _layout       (0)
{
    init();
}

Check_Button_Group::~Check_Button_Group()
{}

void Check_Button_Group::set(const List<bool> & in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    value_update();

    signal.emit(_value);
}

void Check_Button_Group::set_index(const List<int> & in)
{
    const List<int> & index = get_index();

    if (in == index)
    {
        return;
    }

    for (size_t i = 0; i < _value.size(); ++i)
    {
        _value[i] = List_Util::find(static_cast<int>(i), in) != -1;
    }

    value_update();

    signal.emit(_value);
}

const List<bool> & Check_Button_Group::get() const
{
    return _value;
}

const List<int> Check_Button_Group::get_index() const
{
    List<int> out;

    for (size_t i = 0; i < _value.size(); ++i)
        if (_value[i])
        {
            out += static_cast<int>(i);
        }

    return out;
}

void Check_Button_Group::title(const String & in)
{
    if (in == _title)
    {
        return;
    }

    _title = in;

    widget_update();
}

const String & Check_Button_Group::title() const
{
    return _title;
}

void Check_Button_Group::label(const List<String> & in)
{
    if (in == _label)
    {
        return;
    }

    _label = in;

    widget_update();
}

const List<String> & Check_Button_Group::label() const
{
    return _label;
}

void Check_Button_Group::widget_callback(bool)
{
    List<bool> value;

    for (size_t i = 0; i < _widgets.size(); ++i)
    {
        value += _widgets[i]->get();
    }

    set(value);
}

void Check_Button_Group::value_update()
{
    callbacks(false);

    for (
        size_t i = 0;
        i < _value.size() && i < _widgets.size();
        ++i)
    {
        _widgets[i]->set(_value[i]);
    }

    callbacks(true);
}

void Check_Button_Group::widget_update()
{
    callbacks(false);

    // Create widgets.

    _widget_layout->del();

    _widgets.resize(_label.size());

    for (size_t i = 0; i < _widgets.size(); ++i)
    {
        _widgets[i] = new Check_Button(_label[i]);
    }

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
        _widgets[i]->signal.set(this, widget_callback);
    }

    callbacks(true);
}

} // djv

