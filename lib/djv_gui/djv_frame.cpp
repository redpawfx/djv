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

//! \file djv_frame.cpp

#include <djv_frame.h>

#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Frame
//------------------------------------------------------------------------------

Frame::Frame(STYLE style, bool down) :
    select_signal       (this),
    select_off_signal   (this),
    highlight_signal    (this),
    highlight_off_signal(this),
    _style (style),
    _down  (down),
    _margin(0)
{
    debug_path("frame::Frame");

    Fl_Group::box(FL_NO_BOX);
}

Frame::~Frame()
{}

void Frame::style(STYLE in)
{
    if (in == _style)
    {
        return;
    }

    _style = in;

    dirty();
    update();
}

Frame::STYLE Frame::style() const
{
    return _style;
}

void Frame::down(bool in)
{
    if (in == _down)
    {
        return;
    }

    _down = in;

    dirty();
    update();
}

bool Frame::down() const
{
    return _down;
}

void Frame::margin(int in)
{
    if (in == _margin)
    {
        return;
    }

    _margin = in;

    dirty();
    update();
}

int Frame::margin() const
{
    return _margin;
}

int Frame::handle(int in)
{
    //DJV_DEBUG("Frame::handle");

    switch (in)
    {
        case FL_ENTER:
            if (highlight() && active())
            {
                highlight_signal.emit(this);
            }

            break;

        case FL_LEAVE:
            if (highlight() && active())
            {
                highlight_off_signal.emit(this);
            }

            break;
    }

    return Widget::handle(in);
}

void Frame::draw()
{
    //DJV_DEBUG("Frame::draw");

    const Box2i & geom = this->geom();

    //DJV_DEBUG_PRINT("geom = " << geom);
    //DJV_DEBUG_PRINT("margin = " << margin);

    const Box2i box(
        geom.x + _margin,
        geom.y + _margin,
        geom.w - _margin * 2,
        geom.h - _margin * 2);

    //DJV_DEBUG_PRINT("box = " << box);

    fl_draw_box(
        box_type(_style, _down),
        box.x, box.y, box.w, box.h,
        color());

    Fl_Group::draw();
    Abstract_Widget::draw();
}

int Frame::frame_size() const
{
    return Fl::box_dx(box_type(_style, _down)) + _margin;
}

Box2i Frame::frame_geom() const
{
    const Box2i & geom = this->geom();
    const int margin = frame_size();

    return Box2i(
        geom.x + margin,
        geom.y + margin,
        geom.w - margin * 2,
        geom.h - margin * 2);
}

void Frame::dirty()
{
    Widget::dirty();

    const int margin = frame_size();

    size_hint((layout() ? layout()->size_hint() : 0) + margin * 2);
}

void Frame::geom(const Box2i & in)
{
    Fl_Widget::resize(in.x, in.y, in.w, in.h);

    Layout_Item::geom(in);

    if (layout())
    {
        layout()->geom(frame_geom());
    }
}

const Box2i & Frame::geom() const
{
    return Widget::geom();
}

Fl_Boxtype Frame::box_type(STYLE style, bool down)
{
    Fl_Boxtype out = static_cast<Fl_Boxtype>(0);

    switch (style)
    {
        case STYLE_NORMAL:
            out = down ? FL_BOX_DOWN : FL_BOX_UP;
            break;

        case STYLE_H1:
            out = down ? FL_BOX_DOWN_H1 : FL_BOX_UP_H1;
            break;

        case STYLE_H2:
            out = down ? FL_BOX_DOWN_H2 : FL_BOX_UP_H2;
            break;

        case STYLE_H3:
            out = down ? FL_BOX_DOWN_H3 : FL_BOX_UP_H3;
            break;

        case STYLE_V1:
            out = down ? FL_BOX_DOWN_V1 : FL_BOX_UP_V1;
            break;

        case STYLE_V2:
            out = down ? FL_BOX_DOWN_V2 : FL_BOX_UP_V2;
            break;

        case STYLE_V3:
            out = down ? FL_BOX_DOWN_V3 : FL_BOX_UP_V3;
            break;

        case STYLE_THIN:
            out = down ? FL_BOX_THIN_DOWN : FL_BOX_THIN_UP;
            break;

        case STYLE_BORDER:
            out = FL_BOX_BORDER;
            break;

        case STYLE_FLAT:
            out = FL_FLAT_BOX;
            break;

        case STYLE_EMPTY:
            out = FL_NO_BOX;
            break;
    }

    return out;
}

} // djv
