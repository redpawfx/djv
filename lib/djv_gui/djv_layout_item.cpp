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

//! \file djv_layout_item.cpp

#include <djv_layout_item.h>

#include <djv_style.h>

#include <algorithm>

namespace djv
{

//------------------------------------------------------------------------------
// Layout_Item
//------------------------------------------------------------------------------

Layout_Item::Layout_Item(ITEM item) :
    _item(item),
    _parent(0),
    _debug_path("Layout_Item")
{
    _size [0] = _size [1] = STRETCH;
    _align[0] = _align[1] = CENTER;
}

Layout_Item::~Layout_Item()
{}

void Layout_Item::dirty()
{}

void Layout_Item::update()
{
    if (_parent)
    {
        _parent->update();
    }
}

void Layout_Item::geom(const Box2i & in)
{
    _geom = in;
}

void Layout_Item::size_hint(const V2i & in)
{
    _size_hint = in;
}

void Layout_Item::size(SIZE h, SIZE v)
{
    _size[0] = h;
    _size[1] = v;
}

void Layout_Item::align(ALIGN h, ALIGN v)
{
    _align[0] = h;
    _align[1] = v;
}

void Layout_Item::group(const List<Layout_Item *> & in)
{
    _group = in;
}

void Layout_Item::create_group(const List<Layout_Item *> & in)
{
    List<Layout_Item *> tmp, tmp2;

    for (size_t i = 0; i < in.size(); ++i)
    {
        if (in[i]->label_align())
        {
            tmp += in[i]->label_align();
        }

        if (in[i]->group_align())
        {
            tmp2 += in[i]->group_align();
        }
    }

    for (size_t i = 0; i < in.size(); ++i)
    {
        if (in[i]->label_align())
        {
            in[i]->label_align()->group(tmp);
        }

        if (in[i]->group_align())
        {
            in[i]->group_align()->group(tmp2);
        }
    }
}

void Layout_Item::parent(Layout_Item * in)
{
    _parent = in;
}

void Layout_Item::debug_path(const String & in)
{
    _debug_path = in;
}

String Layout_Item::debug_path() const
{
    return _debug_path;
}

String Layout_Item::debug_path(const Layout_Item * in)
{
    List<String> out;

    out += in->debug_path();

    const Layout_Item * tmp = in;

    while ((tmp = tmp->parent()))
    {
        out += tmp->debug_path();
    }

    std::reverse(out.begin(), out.end());

    return String_Util::join(out, '/');
}

//------------------------------------------------------------------------------
// Layout_Spacer
//------------------------------------------------------------------------------

Layout_Spacer::Layout_Spacer(const V2i & size) :
    Layout_Item(SPACER),
    _spacer_size(size)
{
    this->size(FIXED, FIXED);
}

void Layout_Spacer::spacer_size(const V2i & in)
{
    if (in == _spacer_size)
    {
        return;
    }

    _spacer_size = in;

    dirty();
}

const V2i & Layout_Spacer::spacer_size() const
{
    return _spacer_size;
}

void Layout_Spacer::dirty()
{
    Layout_Item::dirty();

    size_hint(V2i(
        (_spacer_size.x != -1) ? _spacer_size.x : Style::global()->spacing(),
        (_spacer_size.y != -1) ? _spacer_size.y : Style::global()->spacing()));
}

} // djv
