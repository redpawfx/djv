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

//! \file djv_gl.h

#ifndef DJV_GL_H
#define DJV_GL_H

#include <djv_color.h>
#include <djv_pixel.h>

#include <djv_box.h>
#include <djv_string.h>
#include <djv_vector.h>

//! \todo This define is needed for static linking with GLEW?

#define GLEW_STATIC

#include <GL/glew.h>
#if defined(DJV_WINDOWS)
#include <GL/wglew.h>
#elif defined(DJV_OSX)
#else
#include <GL/glxew.h>
#endif

#include <FL/gl.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \struct Gl_Util
//!
//! This struct provides OpenGL utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Gl_Util
{
    static void ortho(
        const V2i & size,
        const V2f & min_max = V2f(-1.0, 1.0));

    //! Convert a pixel format to OpenGL.

    static GLenum format(Pixel::PIXEL, bool bgr = false);

    //! Convert a pixel type to OpenGL.

    static GLenum type(Pixel::PIXEL);

    //! Set the current OpenGL color.

    static inline void color(const Color &);

    //! Draw a box.

    static inline void draw_box(const Box2i &);

    //! Draw a box.

    static inline void draw_box(const Box2f &);

    //! Draw a box.

    static inline void draw_box(const Box2i &, const V2f uv [4]);

    //! Draw a box.

    static inline void draw_box(const Box2f &, const V2f uv [4]);
};

//------------------------------------------------------------------------------

//! DJV_DEBUG_GL
//!
//! References:
//!
//! - Dave Shreiner, "Performance OpenGL: Platform Independent Techniques"
//!   SIGGRAPH 2001

#if 0

#define DJV_DEBUG_GL(cmd) \
  \
  cmd; \
  \
  { \
    ::GLenum error = GL_NO_ERROR; \
    \
    if ((error = ::glGetError()) != GL_NO_ERROR) \
    { \
      DJV_DEBUG_PRINT(String_Format("%% = %% (%%, line %%)"). \
        arg(#cmd). \
        arg((char *)::gluErrorString(error)). \
        arg(__FILE__). \
        arg(__LINE__)); \
    } \
  }

#else

#define DJV_DEBUG_GL(cmd) \
  cmd;

#endif

} // djv

#include <djv_gl_inline.h>

#endif // DJV_GL_H

