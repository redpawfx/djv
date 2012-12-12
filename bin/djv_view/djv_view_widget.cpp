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

//! \file djv_view_widget.cpp

#include <djv_view_widget.h>

#include <djv_float_edit.h>
#include <djv_image_prefs.h>
#include <djv_inc_button.h>
#include <djv_menu.h>
#include <djv_prefs.h>
#include <djv_row_layout.h>
#include <djv_text_display.h>
#include <djv_text_edit.h>

#include <djv_file.h>
#include <djv_time.h>

#include <FL/fl_draw.H>

namespace djv_view
{
using djv::Time;

//------------------------------------------------------------------------------
// Frame_Widget
//------------------------------------------------------------------------------

Frame_Widget::Frame_Widget() :
    signal(this),
    _value(0)
{
    // Create widgets.

    _widget = new Text_Edit;

    Inc_Button * inc_widget = new Inc_Button;

    // Layout.

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    Horizontal_Layout * layout = new Horizontal_Layout(this);
    layout->spacing(0);
    layout->margin(0);
    layout->add(_widget);
    layout->stretch(_widget);
    layout->add(inc_widget);

    // Initialize.

    widget_update();

    // Callbacks.

    _widget->signal.set(this, widget_callback);
    inc_widget->inc_signal.set(this, inc_callback);
    inc_widget->dec_signal.set(this, dec_callback);
}

void Frame_Widget::set(int64_t in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();

    signal.emit(_value);
}

int64_t Frame_Widget::get() const
{
    return _value;
}

void Frame_Widget::list(const List<int64_t> & in)
{
    if (in == _list)
    {
        return;
    }

    _list = in;

    widget_update();
}

const List<int64_t> & Frame_Widget::list() const
{
    return _list;
}

void Frame_Widget::speed(const Speed & in)
{
    if (in == _speed)
    {
        return;
    }

    _speed = in;

    widget_update();
}

const Speed & Frame_Widget::speed() const
{
    return _speed;
}

void Frame_Widget::text_font(Fl_Font in)
{
    Widget::text_font(in);

    _widget->text_font(in);
}

Fl_Font Frame_Widget::text_font() const
{
    return Widget::text_font();
}

void Frame_Widget::text_color(Fl_Color in)
{
    Widget::text_color(in);

    _widget->text_color(in);
}

Fl_Color Frame_Widget::text_color() const
{
    return Widget::text_color();
}

namespace
{

const String
    label_timecode_format = "00:00:00:00",
    label_frame_format = "000000";

} // namespace

void Frame_Widget::dirty()
{
    String size_string;

    switch (Time::default_units)
    {
        case Time::UNITS_TIMECODE:
            size_string = label_timecode_format;
            break;

        case Time::UNITS_FRAMES:
            size_string = label_frame_format;
            break;

        default:
            break;
    }

    _widget->size_string(size_string);

    Widget::dirty();

    widget_update();
}

Layout_Item * Frame_Widget::group_align()
{
    return _widget;
}

void Frame_Widget::widget_callback(const String & in)
{
    set(Seq::find_closest(
        Time::string_to_units(in, _speed),
        _list));

    widget_update();
}

void Frame_Widget::inc_callback(bool)
{
    set(_value + 1);
}

void Frame_Widget::dec_callback(bool)
{
    set(_value - 1);
}

void Frame_Widget::widget_update()
{
    callbacks(false);

    int64_t frame = 0;

    if (_list.size() && _value < static_cast<int64_t>(_list.size()))
    {
        frame = _list[_value];
    }

    _widget->set(Time::units_to_string(frame, _speed));

    callbacks(true);
}

//------------------------------------------------------------------------------
// Frame_Slider
//------------------------------------------------------------------------------

Frame_Slider::Frame_Slider() :
    push_signal(this),
    _in_out(true),
    _in(0),
    _out(0)
{
    Fl_Group::clear_visible_focus();
}

void Frame_Slider::in_out(bool in)
{
    _in_out = in;

    redraw();
}

bool Frame_Slider::in_out() const
{
    return _in_out;
}

void Frame_Slider::in_out(int64_t in, int64_t out)
{
    _in = in;
    _out = out;

    redraw();
}

int64_t Frame_Slider::in() const
{
    return _in;
}

int64_t Frame_Slider::out() const
{
    return _out;
}

void Frame_Slider::min_action()
{
    set(_in > min() ? _in : min());
}

void Frame_Slider::max_action()
{
    set(_out < max() ? _out : max());
}

void Frame_Slider::cached_frames(const List<int64_t> & in)
{
    _cached_frames = in;

    redraw();
}

int Frame_Slider::handle(int in)
{
    switch (in)
    {
            //! \todo Is this still necessary?
            //case FL_KEYBOARD: return 0;

        case FL_PUSH:
            push_signal.emit(true);
            break;

        case FL_RELEASE:
            push_signal.emit(false);
            break;
    }

    return Int_Slider::handle(in);
}

void Frame_Slider::draw()
{
    //DJV_DEBUG("Frame_Slider::draw");

    Int_Slider::draw();

    const Box2i & box = slider_geom();

    const bool inside = Abstract_Widget::inside();

    const int64_t min = this->min();
    const int64_t max = this->max();
    const int64_t range = max - min;

    if ((_in > min || _out < max) && min != max)
    {
        const double v0 =
            (Math::max(_in, min) - min) / static_cast<double>(range);
        const double v1 =
            (Math::min(_out, max) - min) / static_cast<double>(range);

        fl_color(_in_out ? FL_SELECTION_COLOR : FL_INACTIVE_COLOR);

        fl_rectf(
            box.x + Math::floor(box.w * v0),
            box.y,
            Math::floor(box.w * (v1 - v0)) + 1,
            box.h);
    }

    if (_cached_frames.size())
    {
        fl_color(FL_BLUE);

        const List<Range<int64_t> > list = Range_Util::range(_cached_frames);

        //DJV_DEBUG_PRINT("cached = " << list.size());

        for (size_t i = 0; i < list.size(); ++i)
        {
            const double v0 =
                (list[i].min - min) / static_cast<double>(range);
            const double v1 =
                (list[i].max - min) / static_cast<double>(range);

            fl_rectf(
                box.x + Math::floor(box.w * v0),
                box.y + box.h / 2,
                Math::floor(box.w * (v1 - v0)) + 1,
                box.h / 2);
        }
    }

    handle_draw(
        min != max ?
        ((get() - min) / static_cast<double>(max - min)) :
        0,
        frame_geom(),
        inside);
}

//------------------------------------------------------------------------------
// Frame_Display_Widget
//------------------------------------------------------------------------------

Frame_Display_Widget::Frame_Display_Widget() :
    _value(0)
{
    // Create widgets.

    _widget = new Text_Display;

    // Layout.

    size(FIXED, FIXED);
    align(LEFT, CENTER);

    Horizontal_Layout * layout = new Horizontal_Layout(this);
    layout->margin(0);
    layout->add(_widget);

    // Initialize.

    widget_update();
}

void Frame_Display_Widget::set(int64_t in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();
}

int64_t Frame_Display_Widget::get() const
{
    return _value;
}

void Frame_Display_Widget::speed(const Speed & in)
{
    if (in == _speed)
    {
        return;
    }

    _speed = in;

    widget_update();
}

const Speed & Frame_Display_Widget::speed() const
{
    return _speed;
}

void Frame_Display_Widget::text_font(Fl_Font in)
{
    Widget::text_font(in);

    _widget->text_font(in);
}

Fl_Font Frame_Display_Widget::text_font() const
{
    return Widget::text_font();
}

void Frame_Display_Widget::text_color(Fl_Color in)
{
    Widget::text_color(in);

    _widget->text_color(in);
}

Fl_Color Frame_Display_Widget::text_color() const
{
    return Widget::text_color();
}

void Frame_Display_Widget::dirty()
{
    String size_string;

    switch (Time::default_units)
    {
        case Time::UNITS_TIMECODE:
            size_string = label_timecode_format;
            break;

        case Time::UNITS_FRAMES:
            size_string = label_frame_format;
            break;

        default:
            break;
    }

    _widget->size_string(size_string);

    Widget::dirty();

    widget_update();
}

void Frame_Display_Widget::widget_update()
{
    callbacks(false);

    _widget->set(Time::units_to_string(_value, _speed));

    callbacks(true);
}

//------------------------------------------------------------------------------
// Speed_Widget
//------------------------------------------------------------------------------

Speed_Widget::Speed_Widget() :
    signal(this)
{
    //_widget = new Text_Display;
    _widget = new Float_Edit;
    _menu = new Popup_Menu;

    size(FIXED, FIXED);

    Horizontal_Layout * layout = new Horizontal_Layout(this);
    layout->spacing(0);
    layout->margin(0);
    layout->add(_widget);
    layout->stretch(_widget);
    layout->add(_menu);

    widget_update();
    menu_update();

    _widget->signal.set(this, widget_callback);
    _menu->signal.set(this, menu_callback);
}

void Speed_Widget::set(const Speed & in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();

    signal.emit(_value);
}

const Speed & Speed_Widget::get() const
{
    return _value;
}

void Speed_Widget::set_default(const Speed & in)
{
    if (in == _value_default)
    {
        return;
    }

    _value_default = in;

    menu_update();
}

const Speed & Speed_Widget::get_default() const
{
    return _value_default;
}

namespace
{

const String
label_speed_format = "000.00";

} // namespace

void Speed_Widget::dirty()
{
    //_widget->size_string(label_speed_format);

    Widget::dirty();
}

void Speed_Widget::widget_callback(double in)
{
    //DJV_DEBUG("Speed_Widget::widget_callback");
    //DJV_DEBUG_PRINT("in = " << in);

    const Speed value = Speed::float_to_speed(in);

    //DJV_DEBUG_PRINT("value = " << value);

    set(value);
}

void Speed_Widget::menu_callback(int in)
{
    if (_value_default.is_valid())
    {
        set(0 == in ? _value_default : static_cast<Speed::FPS>(in - 1));
    }
    else
    {
        set(static_cast<Speed::FPS>(in));
    }
}

void Speed_Widget::widget_update()
{
    callbacks(false);

    /*_widget->set(float_to_string(
        Speed::speed_to_float(_value),
        -2));*/

    _widget->set(Speed::speed_to_float(_value));

    callbacks(true);
}

void Speed_Widget::menu_update()
{
    callbacks(false);

    Menu_Item_Group group;

    if (_value_default.is_valid())
        group.add(
            String_Util::float_to_string(
                Speed::speed_to_float(_value_default),
                -2),
            0, 0, 0, Menu_Item::DIVIDER);

    const List<String> & label = Speed::label_fps();

    for (size_t i = 0; i < label.size(); ++i)
    {
        group.add(label[i]);
    }

    group.end();

    _menu->set(group);

    callbacks(true);
}

//------------------------------------------------------------------------------
// Speed_Display_Widget
//------------------------------------------------------------------------------

Speed_Display_Widget::Speed_Display_Widget() :
    _value(0.0),
    _dropped_frames(false)
{
    _widget = new Text_Display;

    size(FIXED, FIXED);

    Horizontal_Layout * layout = new Horizontal_Layout(this);
    layout->margin(0);
    layout->add(_widget);

    widget_update();
}

void Speed_Display_Widget::set(double in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    widget_update();
}

void Speed_Display_Widget::dropped_frames(bool in)
{
    if (in == _dropped_frames)
    {
        return;
    }

    _dropped_frames = in;

    _widget->text_color(
        _dropped_frames ? FL_SELECTION_COLOR : FL_FOREGROUND_COLOR);
    redraw();
}

void Speed_Display_Widget::dirty()
{
    _widget->size_string(label_speed_format);

    Widget::dirty();
}

void Speed_Display_Widget::widget_update()
{
    callbacks(false);

    _widget->set(String_Util::float_to_string(_value, -2));

    callbacks(true);
}

} // djv_view

