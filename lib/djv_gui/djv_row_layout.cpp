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

//! \file djv_row_layout.cpp

#include <djv_row_layout.h>

#include <djv_style.h>

namespace djv
{

//------------------------------------------------------------------------------
// _Stretch
//------------------------------------------------------------------------------

namespace
{

class _Stretch : public Layout_Spacer
{};

} // namespace

//------------------------------------------------------------------------------
// Abstract_Row_Layout
//------------------------------------------------------------------------------

Abstract_Row_Layout::Abstract_Row_Layout(Layout_Item * parent) :
    Layout(parent),
    _stretch(-1)
{}

Abstract_Row_Layout::~Abstract_Row_Layout()
{}

Layout_Item * Abstract_Row_Layout::add_spacer(const V2i & in, bool stretch)
{
    Layout_Item * item = new Layout_Spacer(in);

    add(item);

    if (stretch)
    {
        this->stretch(item);
    }

    return item;
}

Layout_Item * Abstract_Row_Layout::add_stretch(bool stretch)
{
    Layout_Item * item = new _Stretch;

    add(item);

    if (stretch)
    {
        this->stretch(item);
    }

    return item;
}

void Abstract_Row_Layout::stretch(int in)
{
    _stretch = in;
}

void Abstract_Row_Layout::stretch(const Layout_Item * in)
{
    const List<Layout_Item *> & list = this->list();

    for (size_t i = 0; i < list.size(); ++i)
        if (in == list[i])
        {
            stretch(static_cast<int>(i));
            break;
        }
}

int Abstract_Row_Layout::stretch() const
{
    return _stretch;
}

void Abstract_Row_Layout::layout(
    const List<Layout_Item *> & in,
    List<Box2i> *               out,
    const V2i &                 size,
    DIRECTION                   direction,
    int                         spacing,
    int                         stretch)
{
    //DJV_DEBUG("Abstract_Row_Layout::layout");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("direction = " << direction);
    //DJV_DEBUG_PRINT("stretch = " << stretch);

    const size_t list_size = in.size();
    out->resize(list_size);

    // Calculate minimum sizes.

    int total_length = 0;
    int stretch_count = 0;
    const Layout_Item * prev = 0;

    List<Layout_Item *>::const_iterator i, end = in.end();
    List<Box2i>::iterator j;
    int k;

    for (i = in.begin(), j = out->begin(), k = 0; i != end; ++i, ++j, ++k)
    {
        const Layout_Item * item = *i;

        if (item->visible())
        {
            j->position.zero();
            j->size = item->size_min();

            const List<Layout_Item *> & item_group = item->group();
            List<Layout_Item *>::const_iterator _i = item_group.begin();

            for (
                const List<Layout_Item *>::const_iterator _end =
                    item_group.end();
                _i != _end;
                ++_i)
            {
                if ((*_i) != item && (*_i)->visible())
                {
                    Vector_Util::max((*_i)->size_min(), j->size, &j->size);
                }
            }

            total_length += j->size.e[direction];

            if (item->item() != SPACER && prev)
            {
                if (prev->item() != SPACER)
                {
                    total_length += spacing;
                }
            }

            if (STRETCH == item->size()[direction] || k == stretch)
            {
                ++stretch_count;
            }

            prev = item;
        }
        else
        {
            j->zero();
        }

        //DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j <<
        //  " group = " << item->group().size());
    }

    //DJV_DEBUG_PRINT("total length = " << total_length);

    // Expand to size hints.

    for (i = in.begin(), j = out->begin();
            i != end && (size.e[direction] - total_length) > 0; ++i, ++j)
    {
        const Layout_Item * item = *i;

        if (item->visible())
        {
            const int tmp = j->size.e[direction];

            j->size.e[direction] = Math::min(
                Math::max(item->size_hint().e[direction], j->size.e[direction]),
                tmp + size.e[direction] - total_length);

            total_length += j->size.e[direction] - tmp;

            //DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j);
        }
    }

    //DJV_DEBUG_PRINT("total size = " << total_length);

    const int stretch_size = Math::max(size.e[direction] - total_length, 0);

    //DJV_DEBUG_PRINT("stretch count = " << stretch_count);
    //DJV_DEBUG_PRINT("stretch size = " << stretch_size);

    // Layout items.

    prev = 0;
    
    int position = 0;

    for (i = in.begin(), j = out->begin(), k = 0; i != end; ++i, ++j, ++k)
    {
        const Layout_Item * item = *i;

        if (item->visible())
        {
            if (item->item() != SPACER && prev)
            {
                if (prev->item() != SPACER)
                {
                    position += spacing;
                }
            }

            if (STRETCH == item->size()[direction] || k == stretch)
            {
                int tmp = 0;

                if (k == stretch)
                {
                    tmp = stretch_size;
                }
                else if (-1 == stretch && stretch_count)
                {
                    tmp = stretch_size / stretch_count;
                }

                j->size.e[direction] += tmp;
            }

            prev = item;
        }

        j->position.e[direction] = position;
        
        position += j->size.e[direction];

        //DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j);
    }
}

//------------------------------------------------------------------------------
// Horizontal_Layout
//------------------------------------------------------------------------------

Horizontal_Layout::Horizontal_Layout(Layout_Item * parent) :
    Abstract_Row_Layout(parent)
{
    debug_path("Horizontal_Layout");

    //size(STRETCH, FIXED);
}

void Horizontal_Layout::add_spacer(int in, bool stretch)
{
    Abstract_Row_Layout::add_spacer(V2i(in, 0), stretch);
}

void Horizontal_Layout::dirty()
{
    //DJV_DEBUG("Horizontal_Layout::dirty");

    Abstract_Row_Layout::dirty();

    layout(list(), &_list, V2i(), HORIZONTAL, spacing(), stretch());

    int w = 0, h = 0;

    if (_list.size())
    {
        w = _list[_list.size() - 1].lower_right().x + 1 + margin() * 2;
    }

    for (size_t i = 0; i < _list.size(); ++i)
    {
        h = Math::max(h, _list[i].h);
    }

    _geom_prev.zero();

    size_hint(V2i(w, h + margin() * 2));
}

void Horizontal_Layout::geom(const Box2i & in)
{
    if (in == _geom_prev)
    {
        return;
    }

    _geom_prev = in;

    //DJV_DEBUG("Horizontal_Layout::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    Abstract_Row_Layout::geom(in);

    const List<Layout_Item *> & list = this->list();
    const int margin = this->margin();
    const int margin2 = margin * 2;

    layout(list, &_list, in.size - margin2, HORIZONTAL, spacing(), stretch());

    Box2i geom;
    List<Layout_Item *>::const_iterator i = list.begin();
    List<Box2i>::const_iterator j = _list.begin();

    for (
        const List<Layout_Item *>::const_iterator end = list.end();
        i != end;
        ++i, ++j)
    {
        Layout_Item * item = *i;

        //DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j);

        int _y = j->y, _h = j->h;

        if (STRETCH == item->size()[VERTICAL])
        {
            _h = Math::max(in.h - margin2, _h);
        }
        else
        {
            switch (item->align()[VERTICAL])
            {
                case LEFT:
                    break;

                case CENTER:
                    _y += ((in.h - margin2) - _h) / 2;
                    break;

                case RIGHT:
                    break;
            }
        }
        
        geom.x = in.x + margin + j->x;
        geom.y = in.y + margin + _y;
        geom.w = j->w;
        geom.h = _h;
        item->geom(geom);
    }
}

const Box2i & Horizontal_Layout::geom() const
{
    return Abstract_Row_Layout::geom();
}

//------------------------------------------------------------------------------
// Vertical_Layout
//------------------------------------------------------------------------------

Vertical_Layout::Vertical_Layout(Layout_Item * parent) :
    Abstract_Row_Layout(parent)
{
    debug_path("Vertical_Layout");

    //size(FIXED, STRETCH);
}

void Vertical_Layout::add_spacer(int in, bool stretch)
{
    Abstract_Row_Layout::add_spacer(V2i(0, in), stretch);
}

void Vertical_Layout::dirty()
{
    //DJV_DEBUG("Vertical_Layout::dirty");

    Abstract_Row_Layout::dirty();

    const List<Layout_Item *> & list = this->list();
    layout(list, &_list, V2i(), VERTICAL, spacing(), stretch());

    int w = 0, h = 0;

    for (size_t i = 0; i < _list.size(); ++i)
    {
        w = Math::max(w, _list[i].w);
    }

    if (_list.size())
    {
        h = _list[_list.size() - 1].lower_right().y + 1 + margin() * 2;
    }

    _geom_prev.zero();

    size_hint(V2i(w + margin() * 2, h));
}

void Vertical_Layout::geom(const Box2i & in)
{
    if (in == _geom_prev)
    {
        return;
    }

    _geom_prev = in;

    //DJV_DEBUG("Vertical_Layout::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    Abstract_Row_Layout::geom(in);

    const List<Layout_Item *> & list = this->list();
    const int margin = this->margin();
    const int margin2 = margin * 2;

    layout(list, &_list, in.size - margin2, VERTICAL, spacing(), stretch());

    Box2i geom;
    List<Layout_Item *>::const_iterator i = list.begin();
    List<Box2i>::const_iterator j = _list.begin();

    for (
        const List<Layout_Item *>::const_iterator end = list.end();
        i != end;
        ++i, ++j)
    {
        Layout_Item * item = *i;

        //DJV_DEBUG_PRINT(layout_item::debug_path(item) << " = " << *j);

        int _x = j->x, _w = j->w;

        if (STRETCH == item->size()[HORIZONTAL])
        {
            _w = Math::max(in.w - margin2, _w);
        }
        else
            switch (item->align()[HORIZONTAL])
            {
                case LEFT:
                    break;

                case CENTER:
                    _x += ((in.w - margin2) - _w) / 2;
                    break;

                case RIGHT:
                    break;
            }

        geom.x = in.x + margin + _x;
        geom.y = in.y + margin + j->y;
        geom.w = _w;
        geom.h = j->h;
        item->geom(geom);
    }
}

const Box2i & Vertical_Layout::geom() const
{
    return Abstract_Row_Layout::geom();
}

} // djv

