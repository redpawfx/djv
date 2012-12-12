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

//! \file djv_gl_inline.h

#include <djv_color.h>

#include <djv_box.h>

namespace djv
{

//------------------------------------------------------------------------------
// Gl_Util
//------------------------------------------------------------------------------

inline void Gl_Util::color(const Color & in)
{
    Color tmp(Pixel::RGB_F32);

    Color::convert(in, tmp);

    glColor3f(
        tmp.get_f32(0),
        tmp.get_f32(1),
        tmp.get_f32(2));
}

inline void Gl_Util::draw_box(const Box2i & in)
{
    glVertex2i(in.x, in.y);
    glVertex2i(in.x, in.y + in.h);
    glVertex2i(in.x + in.w, in.y + in.h);
    glVertex2i(in.x + in.w, in.y);
}

inline void Gl_Util::draw_box(const Box2f & in)
{
    glVertex2d(in.x, in.y);
    glVertex2d(in.x, in.y + in.h);
    glVertex2d(in.x + in.w, in.y + in.h);
    glVertex2d(in.x + in.w, in.y);
}

inline void Gl_Util::draw_box(const Box2i & in, const V2f uv [4])
{
    glTexCoord2d(uv[0].x, uv[0].y);
    glVertex2i(in.x, in.y);
    glTexCoord2d(uv[1].x, uv[1].y);
    glVertex2i(in.x, in.y + in.h);
    glTexCoord2d(uv[2].x, uv[2].y);
    glVertex2i(in.x + in.w, in.y + in.h);
    glTexCoord2d(uv[3].x, uv[3].y);
    glVertex2i(in.x + in.w, in.y);
}

inline void Gl_Util::draw_box(const Box2f & in, const V2f uv [4])
{
    glTexCoord2d(uv[0].x, uv[0].y);
    glVertex2d(in.x, in.y);
    glTexCoord2d(uv[1].x, uv[1].y);
    glVertex2d(in.x, in.y + in.h);
    glTexCoord2d(uv[2].x, uv[2].y);
    glVertex2d(in.x + in.w, in.y + in.h);
    glTexCoord2d(uv[3].x, uv[3].y);
    glVertex2d(in.x + in.w, in.y);
}

} // djv

