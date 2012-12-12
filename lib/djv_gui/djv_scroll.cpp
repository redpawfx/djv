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

//! \file djv_scroll.cpp

#include <djv_scroll.h>

#include <djv_row_layout.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>

namespace djv
{

//------------------------------------------------------------------------------
// Scroll
//------------------------------------------------------------------------------

Scroll::Scroll() :
    Fl_Scroll(0, 0, 0, 0),
    _resizable(true)
{
    debug_path("Scroll");

    Fl_Scroll::end();

    Fl_Scroll::box(FL_BOX_THIN_DOWN);
    Fl_Scroll::scrollbar.slider(FL_BOX_THIN_UP);
    Fl_Scroll::hscrollbar.slider(FL_BOX_THIN_UP);

    add(_dummy_widget = new Fl_Box(0, 0, 0, 0));
    _dummy_widget->box(FL_NO_BOX);
    _dummy_widget->hide();

    new Vertical_Layout(this);
}

Scroll::~Scroll()
{}

int Scroll::handle(int in)
{
    switch (Abstract_Widget::handle(in))
    {
        case -1: return 0;
        case  1: return 1;
    }

    return Fl_Scroll::handle(in);
}

void Scroll::show()
{
    Fl_Scroll::show();

    Abstract_Widget::show();
}

void Scroll::draw()
{
    Fl_Scroll::draw();

    Abstract_Widget::draw();
}

void Scroll::resizable(bool in)
{
    _resizable = in;
}

bool Scroll::resizable() const
{
    return _resizable;
}

void Scroll::dirty()
{
    //DJV_DEBUG("Scroll::dirty");

    Abstract_Widget::dirty();

    size_hint(
        V2i(
            Fl_Scroll::scrollbar.w(),
            Fl_Scroll::hscrollbar.h()) +
        1 * 2);

    _size_min = Style::global()->size_text();
}

void Scroll::geom(const Box2i & in)
{
    //DJV_DEBUG("Scroll::geom");
    //DJV_DEBUG_PRINT("in = " << in);

    Fl_Scroll::resize(in.x, in.y, in.w, in.h);

    Layout_Item::geom(in);

    const V2i & size_hint = layout()->size_hint();

    //DJV_DEBUG_PRINT("size hint = " << size_hint);

    const V2i scroll(Fl_Scroll::xposition(), Fl_Scroll::yposition());

    //DJV_DEBUG_PRINT("scroll = " << scroll);

    V2i size;

    if (_resizable)
    {
        size = in.size + scroll;

        //! \todo Scrollbar visibility.

        //size -= V2i(Fl_Scroll::scrollbar.w(), Fl_Scroll::hscrollbar.h());

        if (scroll.y || size_hint.y > size.y)
        {
            size.x -= Fl_Scroll::scrollbar.w();
        }

        if (scroll.x || size_hint.x > size.x)
        {
            size.y -= Fl_Scroll::hscrollbar.h();
        }
    }
    else
    {
        size = size_hint;
    }

    Box2i geom(in.position - scroll, size);

    //! \todo Frame border.

    geom = Box_Util::border(geom, V2i(-1));

    _dummy_widget->resize(geom.x, geom.y, geom.w, geom.h);

    layout()->geom(geom);
}

const Box2i & Scroll::geom() const
{
    return Abstract_Widget::geom();
}

void Scroll::resize(int x, int y, int w, int h)
{
    geom(Box2i(x, y, w, h));
}

const V2i & Scroll::size_min() const
{
    return _size_min;
}

int Scroll::visible() const
{
    return Fl_Scroll::visible();
}

} // djv

