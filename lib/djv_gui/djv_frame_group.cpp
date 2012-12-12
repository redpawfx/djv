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

//! \file djv_frame_group.cpp

#include <djv_frame_group.h>

#include <djv_row_layout.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Abstract_Frame_Group
//------------------------------------------------------------------------------

Abstract_Frame_Group::Abstract_Frame_Group(
    bool                  horizontal,
    const List<Frame *> & group) :
    _horizontal    (horizontal),
    _draw_select   (0),
    _draw_highlight(0),
    _layout        (0)
{
    debug_path("Abstract_Frame_Group");

    size(FIXED, FIXED);

    if (_horizontal)
    {
        _layout = new Horizontal_Layout(this);
    }
    else
    {
        _layout = new Vertical_Layout(this);
    }

    _layout->spacing(1);
    _layout->margin(0);

    this->group(group);

    widget_update();
}

Abstract_Frame_Group::~Abstract_Frame_Group()
{}

void Abstract_Frame_Group::group(const List<Frame *> & in)
{
    //DJV_DEBUG("Abstract_Frame_Group::group");

    _layout->del();

    size_t size = _group.size();

    for (size_t i = 0; i < size; ++i)
    {
        _group[i]->select_signal.del(this);
        _group[i]->select_off_signal.del(this);
        _group[i]->highlight_signal.del(this);
        _group[i]->highlight_off_signal.del(this);
    }

    _group = in;

    size = _group.size();

    for (size_t i = 0; i < size; ++i)
    {
        _group[i]->select_signal.set(this, draw_select_callback);
        _group[i]->select_off_signal.set(this, draw_select_off_callback);
        _group[i]->highlight_signal.set(this, draw_highlight_callback);
        _group[i]->highlight_off_signal.set(this, draw_highlight_off_callback);
    }

    widget_update();
}

const List<Frame *> & Abstract_Frame_Group::group() const
{
    return _group;
}

namespace
{

void draw_edge0(const Box2i & in, bool horizontal)
{
    if (horizontal)
    {
        fl_line(
            in.x - 1,
            in.y,
            in.x - 1,
            in.y + in.h - 1);
    }
    else
    {
        fl_line(
            in.x,
            in.y - 1,
            in.x + in.w - 1,
            in.y - 1);
    }
}

void draw_edge1(const Box2i & in, bool horizontal)
{
    if (horizontal)
    {
        fl_line(
            in.x + in.w,
            in.y,
            in.x + in.w,
            in.y + in.h - 1);
    }
    else
    {
        fl_line(
            in.x,
            in.y + in.h,
            in.x + in.w - 1,
            in.y + in.h);
    }
}

} // namespace

void Abstract_Frame_Group::draw()
{
    //DJV_DEBUG("Abstract_Frame_Group::draw");
    //DJV_DEBUG_PRINT("draw select = " << long(_draw_select));
    //DJV_DEBUG_PRINT("draw highlight = " << long(_draw_highlight));

    const Style::Color & color_value = Style::global()->color_value();
    
    djv::Color tmp(Pixel::RGB_U8);

    djv::Color::scale(0.6, color_value.background, tmp);

    const size_t size = _group.size();

    for (size_t i = 0; i < size - 1; ++i)
    {
        const Box2i & child_geom = _group[i]->geom();

        fl_color(tmp.get_u8(0), tmp.get_u8(1), tmp.get_u8(2));

        draw_edge1(child_geom, _horizontal);
    }

    if (_draw_select && Style_Prefs::global()->focus())
    {
        const Box2i & child_geom = _draw_select->geom();

        fl_color(FL_SELECTION_COLOR);

        if (_draw_select != _group[0])
        {
            draw_edge0(child_geom, _horizontal);
        }

        if (_draw_select != _group[size - 1])
        {
            draw_edge1(child_geom, _horizontal);
        }
    }
    else if (_draw_highlight && Style_Prefs::global()->highlight())
    {
        const Box2i & child_geom = _draw_highlight->geom();

        fl_color(FL_COLOR_HIGHLIGHT);

        if (_draw_highlight != _group[0])
        {
            draw_edge0(child_geom, _horizontal);
        }

        if (_draw_highlight != _group[size - 1])
        {
            draw_edge1(child_geom, _horizontal);
        }
    }

    Widget::draw();
}

void Abstract_Frame_Group::draw_select_callback(const Frame * in)
{
    //DJV_DEBUG("Abstract_Frame_Group::draw_select_callback");
    //DJV_DEBUG_PRINT("in = " << long(in));

    _draw_select = in;

    redraw();
}

void Abstract_Frame_Group::draw_select_off_callback(const Frame * in)
{
    //DJV_DEBUG("Abstract_Frame_Group::draw_select_off_callback");
    //DJV_DEBUG_PRINT("in = " << long(in));

    if (in == _draw_select)
    {
        _draw_select = 0;
    }

    redraw();
}

void Abstract_Frame_Group::draw_highlight_callback(const Frame * in)
{
    //DJV_DEBUG("Abstract_Frame_Group::draw_highlight_callback");
    //DJV_DEBUG_PRINT("in = " << long(in));

    _draw_highlight = in;

    redraw();
}

void Abstract_Frame_Group::draw_highlight_off_callback(const Frame * in)
{
    //DJV_DEBUG("Abstract_Frame_Group::draw_highlight_off_callback");
    //DJV_DEBUG_PRINT("in = " << long(in));

    if (in == _draw_highlight)
    {
        _draw_highlight = 0;
    }

    redraw();
}

void Abstract_Frame_Group::widget_update()
{
    const size_t size = _group.size();

    for (size_t i = 0; i < size; ++i)
    {
        if (0 == i)
        {
            _group[i]->style(_horizontal ? Frame::STYLE_H1 : Frame::STYLE_V1);
        }
        else if ((size - 1) == i)
        {
            _group[i]->style(_horizontal ? Frame::STYLE_H3 : Frame::STYLE_V3);
        }
        else
        {
            _group[i]->style(_horizontal ? Frame::STYLE_H2 : Frame::STYLE_V2);
        }

        _layout->add(_group[i]);
    }
}

//------------------------------------------------------------------------------
// Horizontal_Frame_Group
//------------------------------------------------------------------------------

Horizontal_Frame_Group::Horizontal_Frame_Group(const List<Frame *> & group) :
    Abstract_Frame_Group(true, group)
{}

//------------------------------------------------------------------------------
// Vertical_Frame_Group
//------------------------------------------------------------------------------

Vertical_Frame_Group::Vertical_Frame_Group(const List<Frame *> & group) :
    Abstract_Frame_Group(false, group)
{}

} // djv
