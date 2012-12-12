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

//! \file djv_play2_widget.cpp

#include <djv_play2_widget.h>

#include <djv_application.h>

namespace djv_play2_test
{

Widget::Widget() :
    _image(0)
{}

void Widget::set(const Image * in)
{
    _image = in;
}

void Widget::draw()
{
    Gl_Widget::draw();

    //DJV_DEBUG("Widget::draw");

    const Box2i & geom = this->geom();

    //DJV_DEBUG_PRINT("geom = " << geom);

    Gl_Util::ortho(V2i(geom.w, geom.h));
    DJV_DEBUG_GL(glViewport(0, 0, geom.w, geom.h));

    //DJV_DEBUG_GL(glClearColor(0.0, 0.0, 1.0, 0.0));
    DJV_DEBUG_GL(glClear(GL_COLOR_BUFFER_BIT));

    if (_image)
    {
        const Pixel_Data_Info & info = _image->info();

        //DJV_DEBUG_PRINT("info = " << info);

        try
        {
            _image->bind();
        }
        catch (Error error)
        {
            DJV_APP->error(error);
        }

        DJV_DEBUG_GL(glPushMatrix());

        double u [] = { 0, 0 }, v [] = { 0, 0 };

        u[! info.mirror.x] = info.size.x / static_cast<double>(info.size.x);
        v[! info.mirror.y] = info.size.y / static_cast<double>(info.size.y);

        const V2f uv [] =
        {
            V2f(u[0], v[0]),
            V2f(u[0], v[1]),
            V2f(u[1], v[1]),
            V2f(u[1], v[0])
        };

        DJV_DEBUG_GL(glBegin(GL_QUADS));

        Gl_Util::draw_box(info.size, uv);

        DJV_DEBUG_GL(glEnd());

        DJV_DEBUG_GL(glPopMatrix());
    }
}

} // djv_play2_test

