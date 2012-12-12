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

//! \file djv_glx_context_private.h

#ifndef DJV_GLX_CONTEXT_PRIVATE_H
#define DJV_GLX_CONTEXT_PRIVATE_H

#include <djv_gl_context.h>

#include <GL/gl.h>

#include <X11/Xlib.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Glx_Context_Private
//!
//! This class provides a X11 OpenGL context.
//------------------------------------------------------------------------------

class Glx_Context_Private : public Gl_Context
{
public:

    //! Constructor.

    Glx_Context_Private() throw (Error);

    //! Destructor.

    virtual ~Glx_Context_Private();

    virtual void bind() throw (Error);

    virtual void unbind();

private:

    Display *     _display;
    int           _screen;
    XVisualInfo * _visuals;
    int           _visuals_count;
    Colormap      _colormap;
    Window        _window;
    GLXContext    _context;
};

} // djv

#endif // DJV_GLX_CONTEXT_PRIVATE_H
