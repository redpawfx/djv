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

//! \file djv_columns.cpp

#include <djv_columns.h>

#include <djv_cursor.h>
#include <djv_font.h>
#include <djv_icon.h>
#include <djv_range.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Columns
//------------------------------------------------------------------------------

Columns::Columns() :
    signal(this),
    action_signal(this),
    _sort(-1),
    _sort_reverse(false),
    _push(-1),
    _drag(-1)
{
    //text_font(Style::global()->font_bold());
}

Columns::~Columns()
{}

void Columns::names(const List<String> & in)
{
    if (in == names())
    {
        return;
    }

    //DJV_DEBUG("Columns::names");
    //DJV_DEBUG_PRINT("in = " << in);

    _columns.resize(in.size());

    for (size_t i = 0; i < _columns.size(); ++i)
    {
        _columns[i].name = in[i];
        _columns[i].column = 0;
        _columns[i].size_hint = 0;
    }

    columns_update();
    redraw();

    signal.emit(sizes());
}

void Columns::sizes(const List<int> & in)
{
    if (in == sizes())
    {
        return;
    }

    //DJV_DEBUG("Columns::sizes");
    //DJV_DEBUG_PRINT("in = " << in);

    _columns.resize(in.size());

    const List<int> list = this->sizes();
    int x = 0;

    for (size_t i = 0; i < in.size(); ++i)
    {
        x += in[i] ? in[i] : list[i];
        _columns[i].column = x - 1;
    }

    columns_update();
    redraw();

    signal.emit(sizes());
}

List<String> Columns::names() const
{
    List<String> out(_columns.size());

    for (size_t i = 0; i < _columns.size(); ++i)
    {
        out[i] = _columns[i].name;
    }

    return out;
}

List<int> Columns::sizes() const
{
    List<int> out(_columns.size());
    int x = 0;

    for (size_t i = 0; i < _columns.size(); ++i)
    {
        out[i] = _columns[i].column - x + 1;
        x += out[i];
    }

    return out;
}

void Columns::sort(int in)
{
    if (in == _sort)
    {
        return;
    }

    //DJV_DEBUG("Columns::sort");
    //DJV_DEBUG_PRINT("in = " << in);

    _sort = in;

    redraw();
}

int Columns::sort() const
{
    return _sort;
}

void Columns::sort_reverse(bool in)
{
    if (in == _sort_reverse)
    {
        return;
    }

    //DJV_DEBUG("Columns::sort_reverse");
    //DJV_DEBUG_PRINT("in = " << in);

    _sort_reverse = in;

    redraw();
}

bool Columns::sort_reverse() const
{
    return _sort_reverse;
}

int Columns::handle(int in)
{
    const Box2i & geom = this->geom();
    const V2i mouse(Fl::event_x(), Fl::event_y());

    switch (in)
    {
        case FL_PUSH:
        {
            //DJV_DEBUG("Columns::handle");

            _drag = handle_drag(mouse.x - geom.x, _columns);

            if (-1 == _drag)
            {
                _push = handle_push(mouse.x - geom.x, _columns);
            }

            //DJV_DEBUG_PRINT("push = " << _push);
            //DJV_DEBUG_PRINT("drag = " << _drag);

            redraw();
        }

        return 1;

        case FL_RELEASE:
        {
            const int emit = Box_Util::intersect(geom, mouse) ? _push : -1;
            _push = _drag = -1;

            redraw();

            if (emit != -1)
            {
                action_signal.emit(emit);
            }
        }

        return 1;

        case FL_DRAG:
        {
            if (-1 == _drag)
            {
                break;
            }

            //DJV_DEBUG("Columns::handle");
            //DJV_DEBUG_PRINT("drag = " << _drag);
            //DJV_DEBUG_PRINT("columns = " << sizes());

            const size_t size = _columns.size();

            int min = 0, max = geom.w - 1;

            for (int i = 0; i <= _drag; ++i)
            {
                min += _columns[i].size_hint;
            }

            for (size_t i = _drag + 1; i < size; ++i)
            {
                max -= _columns[i].size_hint;
            }

            _columns[_drag].column = Math::clamp(mouse.x - geom.x, min, max);

            // Push columns.

            for (int i = _drag - 1; i >= 0; --i)
            {
                _columns[i].column = Math::min(
                    _columns[i].column,
                    _columns[i + 1].column - _columns[i + 1].size_hint);
            }

            for (size_t i = _drag + 1; i < size; ++i)
            {
                _columns[i].column = Math::max(
                    _columns[i].column,
                    _columns[i - 1].column + _columns[i].size_hint);
            }

            columns_update();
            redraw();

            signal.emit(sizes());

        }

        return 1;

        case FL_MOVE:
        {
            if (handle_drag(mouse.x - geom.x, _columns) != -1)
            {
                Cursor_Util::cursor(FL_CURSOR_WE);
            }
            else
            {
                Cursor_Util::cursor(FL_CURSOR_DEFAULT);
            }
        }

        return 1;

        case FL_LEAVE:
        {
            Cursor_Util::cursor(FL_CURSOR_DEFAULT);
        }

        return 1;
    }

    return Widget::handle(in);
}

int Columns::handle_push(int in, const List<Column> & list)
{
    for (int i = 0; i < static_cast<int>(list.size()); ++i)
        if (in < list[i].column)
        {
            return i;
        }

    return -1;
}

int Columns::handle_drag(int in, const List<Column> & list)
{
    const int size_pick = Style::global()->size_pick();

    for (int i = 0; i < static_cast<int>(list.size()) - 1; ++i)
        if (
            Range_Util::intersect(
                in,
                Range<int>(
                    list[i].column - size_pick,
                    list[i].column + size_pick)))
        {
            return i;
        }

    return -1;
}

void Columns::draw()
{
    //DJV_DEBUG("Columns::draw");
    //DJV_DEBUG_PRINT("sort = " << _sort);
    //DJV_DEBUG_PRINT("sort reverse = " << _sort_reverse);

    const Box2i & geom = this->geom();
    const List<int> sizes = this->sizes();

    //DJV_DEBUG_PRINT("columns = " << sizes);

    // Background.

    fl_font(text_font(), Style::global()->font_size());
    fl_color(FL_BACKGROUND_COLOR);
    fl_rectf(geom.x, geom.y, geom.w, geom.h);

    const int margin = Style::global()->margin_widget();
    int x = geom.x;

    for (size_t i = 0; i < sizes.size(); ++i)
    {
        fl_draw_box(
            static_cast<int>(i) == _push ? FL_BOX_THIN_DOWN : FL_BOX_THIN_UP,
            x,
            geom.y,
            sizes[i],
            geom.h,
            color());

        // Label.

        fl_color(FL_FOREGROUND_COLOR);
        fl_draw(
            _columns[i].name.c_str(),
            x + margin,
            geom.y + geom.h / 2 - fl_height() / 2 + fl_height() - fl_descent());

        // Sort icon.

        if (static_cast<int>(i) == _sort)
        {
            Fl_Image * icon = Icon_Factory::get_(
                _sort_reverse ? "tool_dec" : "tool_inc");
            
            icon->draw(
                x + sizes[i] - Style::global()->size_tool_icon() - margin,
                geom.y + (geom.h - icon->h()) / 2);
        }

        x += sizes[i];
    }
}

void Columns::dirty()
{
    Widget::dirty();

    fl_font(text_font(), Style::global()->font_size());

    V2i size;

    for (size_t i = 0; i < _columns.size(); ++i)
    {
        const V2i label_size = Font::string_size(_columns[i].name);

        _columns[i].size_hint =
            label_size.x +
            Style::global()->size_tool_icon() +
            Style::global()->margin_widget() * 3;

        size.x += _columns[i].size_hint;
        size.y = Math::max(label_size.y, size.y);
    }

    size_hint(V2i(size.x, size.y + Style::global()->margin_widget() * 2));
}

void Columns::geom(const Box2i & in)
{
    bool dirty = in.size != geom().size;

    Widget::geom(in);

    if (! dirty)
    {
        return;
    }

    //DJV_DEBUG("Columns::geom");
    //DJV_DEBUG_PRINT("width = " << in.w);
    //DJV_DEBUG_PRINT("columns = " << sizes());

    // Resize first column.

    if (_columns.size())
    {
        const int total = _columns[_columns.size() - 1].column;

        for (size_t i = 0; i < _columns.size(); ++i)
        {
            _columns[i].column += in.w - total - 1;
        }
    }

    columns_update();

    signal.emit(sizes());
}

const Box2i & Columns::geom() const
{
    return Widget::geom();
}

void Columns::columns_update()
{
    //DJV_DEBUG("Columns::columns_update");
    //DJV_DEBUG_PRINT("columns = " << sizes());
    //DJV_DEBUG_PRINT("width = " << w());

    const Box2i & geom = this->geom();

    if (geom.w)
    {
        const size_t size = _columns.size();
        List<int> sizes = this->sizes();

        // Check column minimum size.

        for (size_t i = 0; i < size; ++i)
        {
            sizes[i] = Math::max(sizes[i], _columns[i].size_hint);
        }

        //DJV_DEBUG_PRINT("columns = " << sizes);

        // Fit columns within widget.

        int total = 0;

        for (size_t i = 0; i < size; ++i)
        {
            total += sizes[i];
        }

        //DJV_DEBUG_PRINT("total = " << total);

        for (size_t i = 0; i < size && total > geom.w; ++i)
        {
            int tmp = sizes[i];
            sizes[i] = Math::max(
                           sizes[i] - (total - geom.w),
                           _columns[i].size_hint);
            total -= tmp - sizes[i];
        }

        //DJV_DEBUG_PRINT("total = " << total);
        //DJV_DEBUG_PRINT("columns = " << sizes);

        int x = 0;

        for (size_t i = 0; i < size; ++i)
        {
            x += sizes[i];
            _columns[i].column = x - 1;
        }

        //DJV_DEBUG_PRINT("total = " << x);
    }

    //DJV_DEBUG_PRINT("columns = " << sizes());
}

} // djv
