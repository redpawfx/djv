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

//! \file djv_shuttle.cpp

#include <djv_shuttle.h>

#include <djv_cursor.h>
#include <djv_icon.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Image.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Shuttle
//------------------------------------------------------------------------------

Shuttle::Shuttle(const String & prefix) :
    signal(this),
    value_signal(this),
    _value(0.0),
    _pressed(false)
{
    _icon[0] = Icon_Factory::get_(prefix + "shuttle_0");
    _icon[1] = Icon_Factory::get_(prefix + "shuttle_1");
    _icon[2] = Icon_Factory::get_(prefix + "shuttle_2");
    _icon[3] = Icon_Factory::get_(prefix + "shuttle_3");
    _icon[4] = Icon_Factory::get_(prefix + "shuttle_4");
    _icon[5] = Icon_Factory::get_(prefix + "shuttle_5");
    _icon[6] = Icon_Factory::get_(prefix + "shuttle_6");
    _icon[7] = Icon_Factory::get_(prefix + "shuttle_7");

    highlight(true);

    size(FIXED, FIXED);
}

Shuttle::~Shuttle()
{}

int Shuttle::handle(int in)
{
    const V2i mouse(Fl::event_x(), Fl::event_y());

    switch (in)
    {
        case FL_PUSH:

            _value = 0.0;
            _pressed = true;
            _mouse_td = mouse;

            Cursor_Util::cursor(FL_CURSOR_WE);

            redraw();

            signal.emit(true);

            return 1;

        case FL_RELEASE:

            _value = 0;
            _pressed = false;

            Cursor_Util::cursor(FL_CURSOR_DEFAULT);

            redraw();

            signal.emit(false);

            return 1;

        case FL_DRAG:
        {
            const int tmp = mouse.x - _mouse_td.x;

            if (tmp != _value)
            {
                _value = static_cast<double>(tmp);

                redraw();

                value_signal.emit(Math::round(_value / 5.0));
            }
        }

        return 1;
    }

    return Frame::handle(in);
}

void Shuttle::draw()
{
    Widget::draw();

    const Box2i & geom = this->geom();

    const bool inside = Abstract_Widget::inside();

    fl_draw_box(
        box_type(style(), _pressed),
        geom.x, geom.y, geom.w, geom.h,
        inside && active() ? FL_COLOR_HIGHLIGHT : FL_COLOR_BUTTON);

    const int i = Math::mod(Math::round(_value / 5.0), 8);

    _icon[i]->draw(
        geom.x + (geom.w - _icon[0]->w()) / 2,
        geom.y + (geom.h - _icon[0]->h()) / 2);
}

void Shuttle::dirty()
{
    Frame::dirty();

    size_hint(V2i(_icon[0]->w(), _icon[0]->h()) + 3 * 2);
}

} // djv


