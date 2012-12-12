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

//! \file djv_choice_tool_button.cpp

#include <djv_choice_tool_button.h>

#include <djv_icon.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Choice_Tool_Button
//------------------------------------------------------------------------------

Choice_Tool_Button::Choice_Tool_Button(
    const List<String> & icon,
    int                  margin) :
    signal         (this),
    _value         (-1),
    _shortcut      (0),
    _margin        (margin),
    _pressed       (false),
    _pressed_inside(false)
{
    debug_path("Choice_Tool_Button");

    highlight(true);

    size(FIXED, FIXED);

    _icon.resize(icon.size());

    for (size_t i = 0; i < _icon.size(); ++i)
    {
        _icon[i] = Icon_Factory::get_(icon[i]);
    }
}

Choice_Tool_Button::~Choice_Tool_Button()
{}

void Choice_Tool_Button::set(int in)
{
    if (in == _value)
    {
        return;
    }

    //DJV_DEBUG("Choice_Tool_Button::set");
    //DJV_DEBUG_PRINT("in = " << in);

    _value = in;

    dirty();
    redraw();

    signal.emit(_value);
}

int Choice_Tool_Button::get() const
{
    return _value;
}

void Choice_Tool_Button::shortcut(int in)
{
    _shortcut = in;
}

int Choice_Tool_Button::shortcut() const
{
    return _shortcut;
}

int Choice_Tool_Button::handle(int in)
{
    const Box2i & geom = this->geom();
    const V2i mouse(Fl::event_x(), Fl::event_y());

    switch (in)
    {
        case FL_PUSH:
        {
            _pressed = true;
            _pressed_inside = true;

            redraw();
        }

        return 1;

        case FL_RELEASE:
        {
            if (Box_Util::intersect(geom, mouse))
            {
                action();
            }

            _pressed = false;
            _pressed_inside = false;

            redraw();
        }

        return 1;

        case FL_DRAG:
            if (Box_Util::intersect(geom, mouse))
            {
                if (! _pressed_inside)
                {
                    _pressed_inside = true;

                    redraw();
                }
            }
            else
            {
                if (_pressed_inside)
                {
                    _pressed_inside = false;

                    redraw();
                }
            }

            return 1;

        case FL_SHORTCUT:
            if (Fl::event_key() == _shortcut)
            {
                action();

                return 1;
            }

            break;
    }

    return Frame::handle(in);
}

void Choice_Tool_Button::draw()
{
    const Box2i & geom = this->geom();

    const bool inside = Abstract_Widget::inside();

    const bool down = _pressed && _pressed_inside;

    fl_draw_box(
        box_type(style(), down),
        geom.x, geom.y, geom.w, geom.h,
        inside ? FL_COLOR_HIGHLIGHT : FL_COLOR_BUTTON);

    if (_value >= 0 && _value < static_cast<int>(_icon.size()))
    {
        const int x = geom.x + (geom.w - _icon[_value]->w()) / 2;
        const int y = geom.y + (geom.h - _icon[_value]->h()) / 2;

        _icon[_value]->draw(x, y);
    }
}

void Choice_Tool_Button::margin(int in)
{
    _margin = in;
}

int Choice_Tool_Button::margin() const
{
    return _margin;
}

void Choice_Tool_Button::dirty()
{
    Frame::dirty();

    size_hint(
        (
            (_value >= 0 && _value < static_cast<int>(_icon.size())) ?
            V2i(_icon[0]->w(), _icon[0]->h()) :
            V2i(Style::global()->size_icon())
        ) +
        _margin * 2);
}

void Choice_Tool_Button::action()
{
    //DJV_DEBUG("Choice_Tool_Button::action");

    const int value =
        Math::wrap(_value + 1, 0, static_cast<int>(_icon.size()) - 1);

    //DJV_DEBUG_PRINT("value = " << _value);
    //DJV_DEBUG_PRINT("value = " << value);

    set(value);
}

} // djv

