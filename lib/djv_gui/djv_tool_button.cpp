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

//! \file djv_tool_button.cpp

#include <djv_tool_button.h>

#include <djv_icon.h>
#include <djv_row_layout.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Tool_Button
//------------------------------------------------------------------------------

Tool_Button::Tool_Button(const String & icon, int margin) :
    signal(this),
    push_signal(this),
    _value(false),
    _type(PUSH),
    _repeat_active(false),
    _icon(Icon_Factory::get_(icon)),
    _shortcut(0),
    _margin(margin),
    _pressed(false),
    _pressed_inside(false)
{
    debug_path("Tool_Button");

    highlight(true);

    size(FIXED, FIXED);

    if (_icon)
    {
        _icon_inactive =
            std::auto_ptr<Fl_Image>(_icon->copy(_icon->w(), _icon->h()));

        _icon_inactive->inactive();
    }
}

Tool_Button::~Tool_Button()
{}

void Tool_Button::del()
{
    Frame::del();

    Fl::remove_timeout(repeat_callback, this);
}

void Tool_Button::set(bool in)
{
    if (in == _value)
    {
        return;
    }

    switch (_type)
    {
        case TOGGLE:

            _value = in;

            redraw();

            signal.emit(_value);

            break;

        case RADIO:

            _value = in;

            redraw();
            radio_update();

            if (_value)
            {
                signal.emit(_value);
            }

            break;

        default:
            break;
    }
}

bool Tool_Button::get() const
{
    return _value;
}

void Tool_Button::type(TYPE in)
{
    _type = in;
}

Tool_Button::TYPE Tool_Button::type() const
{
    return _type;
}

void Tool_Button::radio_group(const List<Tool_Button *> & in)
{
    _radio_group = in;
}

const List<Tool_Button *> & Tool_Button::radio_group() const
{
    return _radio_group;
}

void Tool_Button::create_radio_group(const List<Tool_Button *> & in)
{
    for (size_t i = 0; i < in.size(); ++i)
    {
        in[i]->radio_group(in);
    }
}

void Tool_Button::icon(Fl_Image * in)
{
    _icon = in;

    _icon_inactive.reset();

    if (_icon)
    {
        _icon_inactive =
            std::auto_ptr<Fl_Image>(_icon->copy(_icon->w(), _icon->h()));

        _icon_inactive->inactive();
    }
}

void Tool_Button::shortcut(int in)
{
    _shortcut = in;
}

int Tool_Button::shortcut() const
{
    return _shortcut;
}

int Tool_Button::handle(int in)
{
    const Box2i & geom = this->geom();
    const V2i mouse(Fl::event_x(), Fl::event_y());

    switch (in)
    {
        case FL_PUSH:
        {
            _pressed = true;
            _pressed_inside = true;

            if (REPEAT == _type)
            {
                Fl::add_timeout(0.5, repeat_callback, this);
            }

            push_signal.emit(true);

            redraw();
        }

        return 1;

        case FL_RELEASE:
        {
            if (! _repeat_active && Box_Util::intersect(geom, mouse))
            {
                action();
            }

            _repeat_active = false;
            _pressed = false;
            _pressed_inside = false;

            Fl::remove_timeout(repeat_callback, this);

            push_signal.emit(false);

            redraw();
        }

        return 1;

        case FL_DRAG:

            if (REPEAT == _type)
            {
                break;
            }

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

                redraw();

                return 1;
            }

            break;
    }

    return Frame::handle(in);
}

void Tool_Button::draw()
{
    //DJV_DEBUG("Tool_Button::draw");

    const Box2i & geom = this->geom();

    const bool inside = Abstract_Widget::inside();

    const bool down = _value || (_pressed && _pressed_inside);

    fl_draw_box(
        box_type(style(), down),
        geom.x, geom.y, geom.w, geom.h,
        inside && active() ? FL_COLOR_HIGHLIGHT : FL_COLOR_BUTTON);

    const int x = geom.x + (geom.w - _icon->w()) / 2;
    const int y = geom.y + (geom.h - _icon->h()) / 2;

    if (active() && _icon)
    {
        _icon->draw(x, y);
    }
    else if (_icon_inactive.get())
    {
        _icon_inactive->draw(x, y);
    }
}

void Tool_Button::margin(int in)
{
    _margin = in;
}

int Tool_Button::margin() const
{
    return _margin;
}

void Tool_Button::dirty()
{
    Frame::dirty();

    size_hint(
        (
            _icon ?
            V2i(_icon->w(), _icon->h()) :
            V2i(Style::global()->size_icon())
        ) +
        _margin * 2);
}

void Tool_Button::repeat_callback()
{
    if (_pressed_inside)
    {
        _repeat_active = true;

        signal.emit(true);

        Fl::repeat_timeout(0.1, repeat_callback, this);
    }
    else
    {
        _repeat_active = false;

        Fl::remove_timeout(repeat_callback, this);
    }
}

void Tool_Button::action()
{
    switch (_type)
    {
        case PUSH:
        case REPEAT:

            signal.emit(true);

            break;

        case TOGGLE:

            _value = ! _value;

            signal.emit(_value);

            break;

        case RADIO:
            if (! _value)
            {
                _value = true;

                radio_update();

                signal.emit(_value);
            }

            break;
    }
}

void Tool_Button::radio_update()
{
    callbacks(false);

    if (_type == RADIO && _value)
        for (size_t i = 0; i < _radio_group.size(); ++i)
            if (_radio_group[i] != this)
            {
                _radio_group[i]->set(false);
            }

    callbacks(true);
}

} // djv

