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

//! \file djv_gl_context.cpp

#include <djv_gl_context.h>

#if defined(DJV_WINDOWS)

#include <djv_wgl_context_private.h>

#elif defined(DJV_OSX)

#include <djv_cgl_context_private.h>

#else

#include <djv_glx_context_private.h>

#endif

namespace djv
{

//------------------------------------------------------------------------------
// Gl_Context
//------------------------------------------------------------------------------

Gl_Context::Gl_Context() throw (Error)
{
    //DJV_DEBUG("Gl_Context::Gl_Context");
}

Gl_Context::~Gl_Context()
{
    //DJV_DEBUG("Gl_Context::~Gl_Context");
}

const String & Gl_Context::vendor() const
{
    return _vendor;
}

const String & Gl_Context::renderer() const
{
    return _renderer;
}

const String & Gl_Context::version() const
{
    return _version;
}

//------------------------------------------------------------------------------
// Gl_Context_Factory
//------------------------------------------------------------------------------

Gl_Context * Gl_Context_Factory::create(bool bind) throw (Error)
{
    Gl_Context * context = 0;

#if defined(DJV_WINDOWS)

    context = new Wgl_Context_Private;

#elif defined(DJV_OSX)

    context = new Cgl_Context_Private;

#else // DJV_WINDOWS

    context = new Glx_Context_Private;

#endif // DJV_WINDOWS

    if (context && bind)
    {
        context->bind();
    }

    return context;
}

} // djv

