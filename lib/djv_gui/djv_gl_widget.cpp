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

//! \file djv_gl_widget.cpp

#include <djv_gl_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
// Gl_Widget
//------------------------------------------------------------------------------

Gl_Widget::Gl_Widget() :
    Fl_Gl_Window(0, 0, 0, 0)
{
    Fl_Gl_Window::end();

    Fl_Gl_Window::mode(FL_RGB | FL_DOUBLE);
}

Gl_Widget::~Gl_Widget()
{}

int Gl_Widget::handle(int in)
{
    switch (Abstract_Widget::handle(in))
    {
        case -1:
            return 0;

        case 1:
            return 1;
    }

    return Fl_Gl_Window::handle(in);
}

void Gl_Widget::show()
{
    Fl_Gl_Window::show();

    Abstract_Widget::show();
}

void Gl_Widget::draw()
{
    //Fl_Gl_Window::draw();

    Abstract_Widget::draw();
}

void Gl_Widget::geom(const Box2i & in)
{
    Fl_Gl_Window::resize(in.x, in.y, in.w, in.h);

    Abstract_Widget::geom(in);
}

const Box2i & Gl_Widget::geom() const
{
    return Abstract_Widget::geom();
}

void Gl_Widget::resize(int x, int y, int w, int h)
{
    geom(Box2i(x, y, w, h));
}

int Gl_Widget::visible() const
{
    return Fl_Gl_Window::visible();
}

} // djv

