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

//! \file djv_gl_offscreen_buffer.cpp

#include <djv_gl_offscreen_buffer.h>

#include <djv_debug.h>

namespace djv
{

//------------------------------------------------------------------------------
// Gl_Offscreen_Buffer
//------------------------------------------------------------------------------

namespace
{

const String
texture_error = "Cannot initialize OpenGL FBO texture",
buffer_init_error = "Cannot initialize OpenGL FBO buffer",
buffer_create_error = "Cannot create OpenGL FBO buffer: %%";

String error_label(GLenum in)
{
    switch (in)
    {
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";

        case GL_FRAMEBUFFER_UNSUPPORTED:
            return "GL_FRAMEBUFFER_UNSUPPORTED";

        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";

        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";

        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
    }

    return String();
}

} // namespace

namespace
{

static int buffer_count = 0;

} // namespace

Gl_Offscreen_Buffer::Gl_Offscreen_Buffer(const Pixel_Data_Info & info)
    throw (Error) :
    _info   (info),
    _id     (0),
    _texture(0),
    _restore(0)
{
    //DJV_DEBUG("Gl_Offscreen_Buffer::Gl_Offscreen_Buffer");
    //DJV_DEBUG_PRINT("info = " << info);

    //DJV_DEBUG_PRINT("buffer_count = " << buffer_count);

    ++buffer_count;

    // Create the texture.

    DJV_DEBUG_GL(glGenTextures(1, &_texture));

    if (! _texture)
    {
        throw Error(texture_error);
    }

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_2D, _texture));

    DJV_DEBUG_GL(glTexParameteri(
                     GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    DJV_DEBUG_GL(glTexParameteri(
                     GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    DJV_DEBUG_GL(glTexParameteri(
                     GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    DJV_DEBUG_GL(glTexParameteri(
                     GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));

    GLenum format = GL_RGBA;

    if (Pixel::F16 == Pixel::type(_info.pixel))
    {
        format = GL_RGBA16F;
    }
    else if (Pixel::F32 == Pixel::type(_info.pixel))
    {
        format = GL_RGBA32F;
    }

    DJV_DEBUG_GL(glTexImage2D(
                     GL_TEXTURE_2D,
                     0,
                     format,
                     _info.size.x,
                     _info.size.y,
                     0,
                     Gl_Util::format(_info.pixel, _info.bgr),
                     Gl_Util::type(_info.pixel),
                     0));

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_2D, 0));

    // Create the FBO.

    DJV_DEBUG_GL(glGenFramebuffers(1, &_id));

    if (! _id)
    {
        throw Error(buffer_init_error);
    }

    Gl_Offscreen_Buffer_Scope scope(this);

    DJV_DEBUG_GL(glFramebufferTexture2D(
                     GL_FRAMEBUFFER,
                     GL_COLOR_ATTACHMENT0,
                     GL_TEXTURE_2D,
                     _texture,
                     0));

    GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (error != GL_FRAMEBUFFER_COMPLETE)
    {
        throw Error(String_Format(buffer_create_error).arg(error_label(error)));
    }

    //DJV_DEBUG_PRINT("id = " << static_cast<int>(_id));
}

Gl_Offscreen_Buffer::~Gl_Offscreen_Buffer()
{
    //DJV_DEBUG("Gl_Offscreen_Buffer::~Gl_Offscreen_Buffer");
    //DJV_DEBUG_PRINT("id = " << static_cast<int>(_id));

    --buffer_count;

    //DJV_DEBUG_PRINT("buffer_count = " << buffer_count);

    if (_id)
    {
        DJV_DEBUG_GL(glDeleteFramebuffers(1, &_id));
    }

    if (_texture)
    {
        DJV_DEBUG_GL(glDeleteTextures(1, &_texture));
    }
}

const Pixel_Data_Info & Gl_Offscreen_Buffer::info() const
{
    return _info;
}

GLuint Gl_Offscreen_Buffer::id() const
{
    return _id;
}

GLuint Gl_Offscreen_Buffer::texture() const
{
    return _texture;
}

void Gl_Offscreen_Buffer::bind()
{
    if (! _id)
    {
        return;
    }

    //DJV_DEBUG("Gl_Offscreen_Buffer::bind");
    //DJV_DEBUG_PRINT("id = " << static_cast<int>(_id));

    DJV_DEBUG_GL(glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_restore));

    //DJV_DEBUG_PRINT("restore = " << static_cast<int>(_restore));

    DJV_DEBUG_GL(glBindFramebuffer(GL_FRAMEBUFFER, _id));
}

void Gl_Offscreen_Buffer::unbind()
{
    if (! _id)
    {
        return;
    }

    //DJV_DEBUG("Gl_Offscreen_Buffer::unbind");
    //DJV_DEBUG_PRINT("id = " << static_cast<int>(_id));
    //DJV_DEBUG_PRINT("restore = " << static_cast<int>(_restore));

    DJV_DEBUG_GL(glBindFramebuffer(GL_FRAMEBUFFER, _restore));

    _restore = 0;
}

//------------------------------------------------------------------------------
// Gl_Offscreen_BufferScope
//------------------------------------------------------------------------------

Gl_Offscreen_Buffer_Scope::Gl_Offscreen_Buffer_Scope(
    Gl_Offscreen_Buffer * buffer) :
    _buffer(buffer)
{
    _buffer->bind();
}

Gl_Offscreen_Buffer_Scope::~Gl_Offscreen_Buffer_Scope()
{
    _buffer->unbind();
}

} // djv
