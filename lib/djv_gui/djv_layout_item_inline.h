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

//! \file djv_layout_item_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// Layout_Item
//------------------------------------------------------------------------------

inline Layout_Item::ITEM Layout_Item::item() const
{
    return _item;
}

inline const Box2i & Layout_Item::geom() const
{
    return _geom;
}

inline const V2i & Layout_Item::size_hint() const
{
    return _size_hint;
}

inline const V2i & Layout_Item::size_min() const
{
    return _size_hint;
}

inline int Layout_Item::visible() const
{
    return true;
}

inline const Layout_Item::SIZE * Layout_Item::size() const
{
    return _size;
}

inline const Layout_Item::ALIGN * Layout_Item::align() const
{
    return _align;
}

inline const List<Layout_Item *> & Layout_Item::group() const
{
    return _group;
}

inline Layout_Item * Layout_Item::group_align()
{
    return this;
}

inline Layout_Item * Layout_Item::label_align()
{
    return 0;
}

inline Layout_Item * Layout_Item::parent() const
{
    return _parent;
}

} // djv
