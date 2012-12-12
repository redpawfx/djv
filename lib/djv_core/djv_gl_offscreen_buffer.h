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

//! \file djv_gl_offscreen_buffer.h

#ifndef DJV_GL_OFFSCREEN_BUFFER_H
#define DJV_GL_OFFSCREEN_BUFFER_H

#include <djv_gl.h>
#include <djv_pixel_data.h>

#include <djv_error.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Gl_Offscreen_Buffer
//!
//! This class provides an OpenGL offscreen buffer.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Gl_Offscreen_Buffer
{
public:

    //! Constructor.

    Gl_Offscreen_Buffer(const Pixel_Data_Info &) throw (Error);

    //! Destructor.

    ~Gl_Offscreen_Buffer();

    //! Get the offscreen buffer information.

    const Pixel_Data_Info & info() const;

    //! Get the offscreen buffer id.

    GLuint id() const;

    //! Get the offscreen buffer texture.

    GLuint texture() const;

    //! Bind the offscreen buffer.

    void bind();

    //! Unbind the offscreen buffer.

    void unbind();

private:

    Gl_Offscreen_Buffer(const Gl_Offscreen_Buffer &);

    Gl_Offscreen_Buffer & operator = (const Gl_Offscreen_Buffer &);

    Pixel_Data_Info _info;
    GLuint          _id;
    GLuint          _texture;
    GLint           _restore;
};

//------------------------------------------------------------------------------
//! \class Gl_Offscreen_Buffer_Scope
//!
//! This class provides automatic binding for an OpenGL offscreen buffer.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Gl_Offscreen_Buffer_Scope
{
public:

    //! Constructor.

    Gl_Offscreen_Buffer_Scope(Gl_Offscreen_Buffer *);

    //! Destructor.

    ~Gl_Offscreen_Buffer_Scope();

private:

    Gl_Offscreen_Buffer * _buffer;
};

} // djv

#endif // DJV_GL_OFFSCREEN_BUFFER_H
