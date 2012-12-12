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

//! \file djv_glsl_util.cpp

#include <djv_glsl_util.h>

#include <djv_box.h>
#include <djv_gl_image.h>

namespace djv_glsl_test
{

namespace
{

String fbo_error(GLenum in)
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

    return "";
}

void fbo_check()
{
    //DJV_DEBUG("fbo_check");

    //DJV_DEBUG_PRINT("error = " <<
    //  fbo_error(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
}

} // namespace

Offscreen::Offscreen() :
    _id(0)
{}

Offscreen::~Offscreen()
{
    if (_id)
    {
        glDeleteFramebuffers(1, &_id);
    }
}

void Offscreen::init() throw (Error)
{
    if (_id)
    {
        return;
    }

    //DJV_DEBUG("Offscreen::init");

    DJV_DEBUG_GL(glGenFramebuffers(1, &_id));

    if (! _id)
    {
        throw Error("Offscreen create.");
    }
}

void Offscreen::bind()
{
    //DJV_DEBUG("Offscreen::bind");

    DJV_DEBUG_GL(glBindFramebuffer(GL_FRAMEBUFFER, _id));
}

void Offscreen::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Offscreen::set(const Texture & in)
{
    //DJV_DEBUG("Offscreen::set");

    fbo_check();

    DJV_DEBUG_GL(glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_RECTANGLE,
        in.id(),
        0));
}

Shader::Shader() :
    _vertex(0),
    _fragment(0),
    _program(0)
{}

Shader::~Shader()
{
    del();
}

void Shader::init(const String & in) throw (Error)
{
    //DJV_DEBUG("Shader::init");
    //DJV_DEBUG_PRINT("in = " << in);

    GLint error = 0;

    del();

    DJV_DEBUG_GL(_vertex = glCreateShader(GL_VERTEX_SHADER));

    if (! _vertex)
    {
        throw Error("Vertex shader create.");
    }

    const String vertex_src =
        "void main(void)\n"
        "{\n"
        "  gl_FrontColor  = gl_Color;\n"
        "  gl_TexCoord[0] = gl_MultiTexCoord0;\n"
        "  gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
        "}\n";
    const char * vertex_sources [] = { vertex_src.c_str() };
    DJV_DEBUG_GL(glShaderSource(_vertex, 1, vertex_sources, 0));
    DJV_DEBUG_GL(glCompileShader(_vertex));
    char log [cstring_size] = "";
    glGetShaderInfoLog(_vertex, cstring_size, 0, log);
    //DJV_DEBUG_PRINT("log = " << log);
    glGetShaderiv(_vertex, GL_COMPILE_STATUS, &error);

    if (! error)
    {
        throw Error(
            String(String_Format("Vertex shader compile: %%").arg(log)));
    }

    DJV_DEBUG_GL(_fragment = glCreateShader(GL_FRAGMENT_SHADER));

    if (! _fragment)
    {
        throw Error("Fragment shader create.");
    }

    const char * fragment_sources [] = { in.c_str() };
    DJV_DEBUG_GL(glShaderSource(_fragment, 1, fragment_sources, 0));
    DJV_DEBUG_GL(glCompileShader(_fragment));
    glGetShaderInfoLog(_fragment, cstring_size, 0, log);
    //DJV_DEBUG_PRINT("log = " << log);
    glGetShaderiv(_fragment, GL_COMPILE_STATUS, &error);

    if (! error)
    {
        throw Error(
            String(String_Format("Fragment shader compile: %%").arg(log)));
    }

    DJV_DEBUG_GL(_program = glCreateProgram());
    DJV_DEBUG_GL(glAttachShader(_program, _vertex));
    DJV_DEBUG_GL(glAttachShader(_program, _fragment));
    DJV_DEBUG_GL(glLinkProgram(_program));
    glGetProgramInfoLog(_program, cstring_size, 0, log);
    //DJV_DEBUG_PRINT("log = " << log);
    glGetProgramiv(_program, GL_LINK_STATUS, &error);

    if (! error)
    {
        throw Error(String(String_Format("Shader compile: %%").arg(log)));
    }
}

void Shader::bind()
{
    //DJV_DEBUG("Shader::bind");

    DJV_DEBUG_GL(glUseProgram(_program));
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::del()
{
    if (_vertex)
    {
        glDeleteShader(_vertex);
        _vertex = 0;
    }

    if (_fragment)
    {
        glDeleteShader(_fragment);
        _fragment = 0;
    }

    if (_program)
    {
        glDeleteShader(_program);
        _program = 0;
    }
}

GLuint Shader::program() const
{
    return _program;
}

Texture::Texture() :
    _min(GL_LINEAR),
    _mag(GL_LINEAR),
    _id(0)
{}

Texture::~Texture()
{
    del();
}

void Texture::init(const Pixel_Data_Info & in, GLenum min, GLenum mag)
    throw (Error)
{
    if (in == _info && min == _min && mag == _mag)
    {
        return;
    }

    //DJV_DEBUG("Texture::init");
    //DJV_DEBUG_PRINT("in = " << in);

    _info = in;
    _min = min;
    _mag = mag;

    del();

    DJV_DEBUG_GL(glGenTextures(1, &_id));

    if (! _id)
    {
        throw Error("Texture create.");
    }

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_RECTANGLE, _id));
    DJV_DEBUG_GL(glTexParameteri(
        GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP));
    DJV_DEBUG_GL(glTexParameteri(
        GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP));
    DJV_DEBUG_GL(glTexParameteri(
        GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, _mag));
    DJV_DEBUG_GL(glTexParameteri(
        GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, _min));

    GLenum format = GL_RGBA;

    if (
        Pixel::F16 == Pixel::type(_info.pixel) ||
        Pixel::F32 == Pixel::type(_info.pixel))
    {
        format = GL_RGBA32F;
    }

    DJV_DEBUG_GL(glTexImage2D(
        GL_TEXTURE_RECTANGLE,
        0,
        format,
        _info.size.x,
        _info.size.y,
        0,
        Gl_Util::format(_info.pixel, _info.bgr),
        Gl_Util::type(_info.pixel),
        0));
}

void Texture::init(const Pixel_Data & in, GLenum min, GLenum mag)
    throw (Error)
{
    //DJV_DEBUG("Texture::init");
    //DJV_DEBUG_PRINT("in = " << in);

    init(in.info(), min, mag);
    bind();
    copy(in);
}

void Texture::del()
{
    if (_id)
    {
        glDeleteTextures(1, &_id);
        _id = 0;
    }
}

void Texture::copy(const Pixel_Data & in)
{
    //DJV_DEBUG("Texture::copy");
    //DJV_DEBUG_PRINT("in = " << in);

    const Pixel_Data_Info & info = in.info();

    Gl_Image::state_unpack(in.info());

    DJV_DEBUG_GL(glTexSubImage2D(
        GL_TEXTURE_RECTANGLE,
        0,
        0,
        0,
        info.size.x,
        info.size.y,
        Gl_Util::format(info.pixel, info.bgr),
        Gl_Util::type(info.pixel),
        in.data()));
}

void Texture::copy(const Pixel_Data & in, const Box2i & area)
{
    //DJV_DEBUG("Texture::copy");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("area = " << area);

    const Pixel_Data_Info & info = in.info();

    V2i position = area.position;

    if (info.mirror.x)
    {
        position.x = info.size.x - area.position.x - area.size.x;
    }

    if (info.mirror.y)
    {
        position.y = info.size.y - area.position.y - area.size.y;
    }

    Gl_Image::state_unpack(in.info(), position);

    DJV_DEBUG_GL(glTexSubImage2D(
        GL_TEXTURE_RECTANGLE,
        0,
        0,
        0,
        area.size.x,
        area.size.y,
        Gl_Util::format(info.pixel, info.bgr),
        Gl_Util::type(info.pixel),
        in.data()));
}

void Texture::copy(const V2i & in)
{
    //DJV_DEBUG("Texture::copy");
    //DJV_DEBUG_PRINT("in = " << in);

    DJV_DEBUG_GL(glCopyTexSubImage2D(
        GL_TEXTURE_RECTANGLE,
        0,
        0,
        0,
        0,
        0,
        in.x,
        in.y));
}

void Texture::bind()
{
    //DJV_DEBUG("Texture::bind");

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_RECTANGLE, _id));
}

const Pixel_Data_Info & Texture::info() const
{
    return _info;
}
GLenum Texture::min() const
{
    return _min;
}
GLenum Texture::mag() const
{
    return _mag;
}
GLuint Texture::id() const
{
    return _id;
}

Kernel::Kernel() :
    _size(0)
{}

namespace
{

const String kernel_src =
    "const int kernel_max = 256;\n"
    "const int kernel_size = %%;\n"
    "uniform float kernel_value [kernel_max];\n"
    "uniform vec2 kernel_offset [kernel_max];\n"
    "\n"
    "vec4 kernel(sampler2DRect texture)\n"
    "{\n"
    "  vec4 tmp = vec4(0.0);\n"
    "  int i;\n"
    "  for (i = 0; i < kernel_size; ++i)\n"
    "  {\n"
    "    tmp += "
    "      kernel_value[i] * "
    "      texture2DRect(texture, gl_TexCoord[0].st + kernel_offset[i]);\n"
    "  }\n"
    "  return tmp;\n"
    "}\n";

} // namespace

void Kernel::init(int size)
{
    _size = size;

    _src = String_Format(kernel_src).arg(_size);
}

const String & Kernel::src() const
{
    return _src;
}

//void Kernel::size(int in)

void Kernel::value(GLuint program, const float * in)
{
    glUniform1fv(glGetUniformLocation(program, "kernel_value"), _size, in);
}

void Kernel::offset(GLuint program, const float * in)
{
    glUniform2fv(glGetUniformLocation(program, "kernel_offset"), _size, in);
}

void Util::quad(const Pixel_Data_Info & in)
{
    double u [] = { 0, 0 }, v [] = { 0, 0 };
    u[! in.mirror.x] = in.size.x;
    v[! in.mirror.y] = in.size.y;
    const V2f uv [] =
    {
        V2f(u[0], v[0]),
        V2f(u[0], v[1]),
        V2f(u[1], v[1]),
        V2f(u[1], v[0])
    };

    glBegin(GL_QUADS);
    Gl_Util::draw_box(Box2i(in.size), uv);
    glEnd();
}

} // djv_glsl_test
