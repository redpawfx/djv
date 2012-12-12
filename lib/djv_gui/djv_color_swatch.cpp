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

//! \file djv_color_swatch.cpp

#include <djv_color_swatch.h>

#include <djv_color_dialog.h>
#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Color_Swatch
//------------------------------------------------------------------------------

Color_Swatch::Color_Swatch() :
    signal       (this),
    action_signal(this)
{
    debug_path("Color_Swatch");

    Fl_Group::clear_visible_focus();

    style(Frame::STYLE_BORDER);

    // Layout.

    size(FIXED, FIXED);
    align(LEFT, LEFT);
}

Color_Swatch::~Color_Swatch()
{}

void Color_Swatch::set(const Color & in)
{
    if (in == _value)
    {
        return;
    }

    _value = in;

    redraw();

    signal.emit(_value);
}

const Color & Color_Swatch::get() const
{
    return _value;
}

int Color_Swatch::handle(int in)
{
    //DJV_DEBUG("Color_Swatch::handle");

    switch (in)
    {
        case FL_FOCUS:
        case FL_UNFOCUS:
            if (visible_focus())
            {
                redraw();

                return 1;
            }

            break;

        case FL_KEYBOARD:
            if (this == Fl::focus())
                switch (Fl::event_key())
                {
                    case ' ':

                        action_signal.emit(true);

                        redraw();

                        return 1;
                }

            break;

        case FL_PUSH:
        {
            if (visible_focus())
            {
                take_focus();
            }

            action_signal.emit(true);

            redraw();

        }

        return 1;
    }

    return Frame::handle(in);
}

void Color_Swatch::draw()
{
    //DJV_DEBUG("Color_Swatch::draw");

    const Box2i & geom = frame_geom();

    const bool inside = Abstract_Widget::inside();
    const bool select = visible_focus() && this == Fl::focus() && active();
    const bool highlight = this->highlight() && inside && active();

    //DJV_DEBUG_PRINT("inside = " << inside);
    //DJV_DEBUG_PRINT("select = " << select);
    //DJV_DEBUG_PRINT("highlight = " << highlight);

    Color tmp(Pixel::RGB_U8);
    Color::convert(_value, tmp);
    fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));
    fl_rectf(geom.x, geom.y, geom.w, geom.h);

    fl_draw_box(
        box_type(style(), false),
        geom.x, geom.y, geom.w, geom.h,
        select ?
        FL_SELECTION_COLOR :
        (highlight ? FL_COLOR_HIGHLIGHT : FL_BACKGROUND_COLOR));

    Widget::draw();
}

void Color_Swatch::dirty()
{
    Frame::dirty();

    size_hint(V2i(Style::global()->size_swatch()) + frame_size() * 2);
}

//------------------------------------------------------------------------------
// Color_Swatch_Widget
//------------------------------------------------------------------------------

Color_Swatch_Widget::Color_Swatch_Widget(const String & label) :
    signal (this),
    _widget(0),
    _label (0),
    _layout(0)
{
    _widget = new Color_Swatch;
    _widget->Fl_Group::set_visible_focus();
    _widget->highlight(true);

    _label = new Label(label);

    size(FIXED, FIXED);
    align(LEFT, LEFT);

    _layout = new Vertical_Layout(this);
    _layout->margin(0);

    if (! label.empty())
    {
        _layout->add(_label);
    }

    _layout->add(_widget);

    _widget->signal.set(this, widget_callback);
    _widget->action_signal.set(this, dialog_callback);
}

Color_Swatch_Widget::~Color_Swatch_Widget()
{}

void Color_Swatch_Widget::set(const Color & in)
{
    _widget->set(in);
}

const Color & Color_Swatch_Widget::get() const
{
    return _widget->get();
}

void Color_Swatch_Widget::label(const String & in)
{
    if (! in.empty())
    {
        _layout->insert(_label, 0);
    }
    else
    {
        _layout->remove(_label);
    }

    _label->set(in);
}

const String & Color_Swatch_Widget::label() const
{
    return _label->get();
}

void Color_Swatch_Widget::widget_callback(const Color & in)
{
    _widget->set(in);

    signal.emit(in);
}

void Color_Swatch_Widget::dialog_callback(bool)
{
    Color_Dialog::dialog(get(), this, widget_callback);
}

} // djv
