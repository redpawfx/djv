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

//! \file djv_widget.cpp

#include <djv_widget.h>

#include <djv_application.h>
#include <djv_style.h>

#include <FL/Fl.H>

#include <iostream>

namespace djv
{

//------------------------------------------------------------------------------
// Widget
//------------------------------------------------------------------------------

Widget::Widget() :
    Fl_Group(0, 0, 0, 0),
    _highlight(false),
    _visible  (false)
{
    debug_path("Widget");

    //DJV_DEBUG("Widget::Widget");

    Fl_Group::end();

    Fl_Group::box(FL_NO_BOX);
}

Widget::~Widget()
{
    //DJV_DEBUG("Widget::~Widget");
}

void Widget::highlight(bool in)
{
    _highlight = in;
}

bool Widget::highlight() const
{
    return _highlight;
}

void Widget::tooltip(const String & in)
{
    Abstract_Widget::tooltip(in);

    const String::size_type size = in.size() + 1;
    _tooltip_cstr.size(size);
    String_Util::cstring(in, _tooltip_cstr(), size);
    Fl_Group::tooltip(_tooltip_cstr());
}

int Widget::handle(int in)
{
    //DJV_DEBUG("Widget::handle");

    switch (Abstract_Widget::handle(in))
    {
        case -1:
            return 0;

        case  1:
            return 1;
    }

    switch (in)
    {
        case FL_ENTER:
        case FL_LEAVE:
            if (_highlight)
            {
                //DJV_DEBUG_PRINT("highlight");

                redraw();
            }

            break;
    }

    return Fl_Group::handle(in);
}

void Widget::show()
{
    Fl_Group::show();

    Abstract_Widget::show();
}

void Widget::draw()
{
    Fl_Group::draw();

    Abstract_Widget::draw();
}

void Widget::dirty()
{
    Abstract_Widget::dirty();

    labelfont(text_font());
    labelsize(Style::global()->font_size());
    labelcolor(text_color());

    _visible = Fl_Group::visible() ? true : false;
}

void Widget::geom(const Box2i & in)
{
    Fl_Widget::resize(in.x, in.y, in.w, in.h);

    Abstract_Widget::geom(in);
}

void Widget::size(SIZE x, SIZE y)
{
    Abstract_Widget::size(x, y);
}

void Widget::align(ALIGN x, ALIGN y)
{
    Abstract_Widget::align(x, y);
}

void Widget::debug_children(const Fl_Group * in, int indent)
{
    const String indent_(' ', indent);

    const Fl_Widget * const * children = in->array();
    const int size = in->children();

    std::cout << indent_ << "{" << std::endl;

    for (int i = 0; i < size; ++i)
    {
        if (const Abstract_Widget * widget = DJV_APP->widget_get(children[i]))
        {
            std::cout << indent_ << "  " << widget->debug_path() << std::endl;
        }
        else
        {
            std::cout << indent_ << "  " << long(children[i]) << std::endl;
        }

        if (const Fl_Group * group =
            dynamic_cast<const Fl_Group *>(children[i]))
        {
            if (group->children())
            {
                debug_children(group, indent + 2);
            }
        }
    }

    std::cout << indent_ << "}" << std::endl;
}

} // djv

