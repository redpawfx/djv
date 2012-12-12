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

//! \file djv_gl_context.h

#ifndef DJV_GL_CONTEXT_H
#define DJV_GL_CONTEXT_H

#include <djv_gl.h>

#include <djv_error.h>

namespace djv
{

class Gl_Context_Factory;

//------------------------------------------------------------------------------
//! \class Gl_Context
//!
//! This class provides an OpenGL device.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Gl_Context
{
public:

    //! Destructor.

    virtual ~Gl_Context() = 0;

    //! Bind the context.

    virtual void bind() throw (Error) = 0;

    //! Unbind the context.

    virtual void unbind() = 0;

    //! Get the vendor.

    const String & vendor() const;

    //! Get the renderer.

    const String & renderer() const;

    //! Get the version.

    const String & version() const;

protected:

    //! Constructor.

    Gl_Context() throw (Error);

    String _vendor;
    String _renderer;
    String _version;

private:

    friend class Gl_Context_Factory;
};

//------------------------------------------------------------------------------
//! \class Gl_Context_Factory
//!
//! This class provides a factory for OpenGL contexts.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Gl_Context_Factory
{
public:

    //! Create an OpenGL context.

    static Gl_Context * create(bool bind = true) throw (Error);
};

} // djv

#endif // DJV_GL_CONTEXT_H

