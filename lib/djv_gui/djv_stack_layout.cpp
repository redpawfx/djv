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

//! \file djv_stack_layout.cpp

#include <djv_stack_layout.h>

namespace djv
{

//------------------------------------------------------------------------------
// Stack_Layout
//------------------------------------------------------------------------------

Stack_Layout::Stack_Layout(Layout_Item * parent) :
    Layout(parent)
{}

Stack_Layout::~Stack_Layout()
{}

void Stack_Layout::dirty()
{
    //DJV_DEBUG("Stack_Layout::dirty");

    Layout::dirty();

    V2i size;
    const List<Layout_Item *> & list = this->list();

    for (size_t i = 0; i < list.size(); ++i)
    {
        size = Vector_Util::max(list[i]->size_hint(), size);
    }

    size_hint(size + margin() * 2);
}

void Stack_Layout::geom(const Box2i & in)
{
    //DJV_DEBUG("Stack_Layout::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    Layout::geom(in);

    const List<Layout_Item *> & list = this->list();

    for (size_t i = 0; i < list.size(); ++i)
    {
        list[i]->geom(Box2i(
            in.x + margin(),
            in.y + margin(),
            in.w - margin() * 2,
            in.h - margin() * 2));
    }
}

const Box2i & Stack_Layout::geom() const
{
    return Layout::geom();
}

} // djv

