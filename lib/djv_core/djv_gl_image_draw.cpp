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

//! \file djv_gl_image_draw.cpp

#include <djv_gl_image.h>

#include <djv_gl_offscreen_buffer.h>

#include <djv_matrix.h>
#include <djv_vector.h>

#include <djv_debug.h>

namespace djv
{

namespace
{

#define _ERROR(in) \
    \
    Error(String_Format("Gl_Image %%%%:%%"). \
        arg(in). \
        arg(__FILE__). \
        arg(__LINE__))

} // namespace

//------------------------------------------------------------------------------
// Gl_Image_Lut
//------------------------------------------------------------------------------

class Gl_Image_Lut
{
public:

    Gl_Image_Lut();

    ~Gl_Image_Lut();

    void init(const Pixel_Data_Info &) throw (Error);

    void init(const Pixel_Data &) throw (Error);

    void copy(const Pixel_Data &);

    void bind();

    void unbind();

    const Pixel_Data_Info & info() const;

    GLuint id() const;

private:

    void del();

    Pixel_Data_Info _info;
    int             _size;
    GLuint          _id;
};

Gl_Image_Lut::Gl_Image_Lut() :
    _size(0),
    _id  (0)
{}

Gl_Image_Lut::~Gl_Image_Lut()
{
    del();
}

void Gl_Image_Lut::init(const Pixel_Data_Info & info) throw (Error)
{
    if (info == _info)
    {
        return;
    }

    //DJV_DEBUG("Gl_Image_Lut::init");
    //DJV_DEBUG_PRINT("info = " << info);

    del();

    _info = info;
    _size = Math::to_pow2(_info.size.x);

    //DJV_DEBUG_PRINT("size = " << _size);

    DJV_DEBUG_GL(glGenTextures(1, &_id));

    if (! _id)
    {
        throw _ERROR("Cannot create texture");
    }

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_1D, _id));
    DJV_DEBUG_GL(
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    DJV_DEBUG_GL(
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    DJV_DEBUG_GL(
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLenum format = GL_RGBA;

    if (Pixel::F16 == Pixel::type(_info.pixel))
    {
        format = GL_RGBA16F;
    }
    else if (Pixel::F32 == Pixel::type(_info.pixel))
    {
        format = GL_RGBA32F;
    }

    DJV_DEBUG_GL(
        glTexImage1D(
            GL_TEXTURE_1D,
            0,
            format,
            _size,
            0,
            Gl_Util::format(_info.pixel, _info.bgr),
            Gl_Util::type(_info.pixel),
            0));
}

void Gl_Image_Lut::init(const Pixel_Data & data) throw (Error)
{
    init(data.info());

    bind();

    copy(data);
}

void Gl_Image_Lut::copy(const Pixel_Data & in)
{
    //DJV_DEBUG("Gl_Image_Lut::copy");
    //DJV_DEBUG_PRINT("in = " << in);

    const Pixel_Data_Info & info = in.info();

    Gl_Image::state_unpack(in.info());

    DJV_DEBUG_GL(
        glTexSubImage1D(
            GL_TEXTURE_1D,
            0,
            0,
            info.size.x,
            Gl_Util::format(info.pixel, info.bgr),
            Gl_Util::type(info.pixel),
            in.data()));
}

void Gl_Image_Lut::bind()
{
    //DJV_DEBUG("Gl_Image_Lut::bind");

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_1D, _id));
}

const Pixel_Data_Info & Gl_Image_Lut::info() const
{
    return _info;
}

GLuint Gl_Image_Lut::id() const
{
    return _id;
}

void Gl_Image_Lut::del()
{
    if (_id)
    {
        glDeleteTextures(1, &_id);

        _id = 0;
    }
}

//------------------------------------------------------------------------------
// Gl_Image_Texture
//------------------------------------------------------------------------------

class Gl_Image_Texture
{
public:

    Gl_Image_Texture();

    ~Gl_Image_Texture();

    void init(
        const Pixel_Data_Info &,
        GLenum = GL_LINEAR,
        GLenum = GL_LINEAR) throw (Error);

    void init(const Pixel_Data &, GLenum = GL_LINEAR, GLenum = GL_LINEAR)
    throw (Error);

    void copy(const Pixel_Data &);

    void copy(const V2i &);

    void bind();

    const Pixel_Data_Info & info() const;

    GLenum min() const;

    GLenum mag() const;

    GLuint id() const;

private:

    void del();

    Pixel_Data_Info _info;
    GLenum          _min;
    GLenum          _mag;
    GLuint          _id;
};

Gl_Image_Texture::Gl_Image_Texture() :
    _min(GL_LINEAR),
    _mag(GL_LINEAR),
    _id (0)
{}

Gl_Image_Texture::~Gl_Image_Texture()
{
    del();
}

void Gl_Image_Texture::init(
    const Pixel_Data_Info & info,
    GLenum                  min,
    GLenum                  mag) throw (Error)
{
    if (info == _info && min == _min && mag == _mag)
    {
        return;
    }

    del();

    //DJV_DEBUG("Gl_Image_Texture::init");
    //DJV_DEBUG_PRINT("info = " << info);

    _info = info;
    _min  = min;
    _mag  = mag;

    DJV_DEBUG_GL(glGenTextures(1, &_id));

    //DJV_DEBUG_PRINT("id = " << int(_id));

    if (! _id)
    {
        throw _ERROR("Cannot create texture");
    }

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_2D, _id));
    DJV_DEBUG_GL(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    DJV_DEBUG_GL(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    DJV_DEBUG_GL(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _min));
    DJV_DEBUG_GL(
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _mag));

    GLenum format = GL_RGBA;

    if (Pixel::F16 == Pixel::type(_info.pixel))
    {
        format = GL_RGBA16F;
    }
    else if (Pixel::F32 == Pixel::type(_info.pixel))
    {
        format = GL_RGBA32F;
    }

    DJV_DEBUG_GL(
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            _info.size.x,
            _info.size.y,
            0,
            Gl_Util::format(_info.pixel, _info.bgr),
            Gl_Util::type(_info.pixel),
            0));
}

void Gl_Image_Texture::init(
    const Pixel_Data & data,
    GLenum             min,
    GLenum             mag) throw (Error)
{
    init(data.info(), min, mag);

    bind();

    copy(data);
}

void Gl_Image_Texture::copy(const Pixel_Data & in)
{
    //DJV_DEBUG("Gl_Image_Texture::copy");
    //DJV_DEBUG_PRINT("in = " << in);

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_2D, _id));

    const Pixel_Data_Info & info = in.info();

    Gl_Image::state_unpack(in.info());

    DJV_DEBUG_GL(
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,
            0,
            info.size.x,
            info.size.y,
            Gl_Util::format(info.pixel, info.bgr),
            Gl_Util::type(info.pixel),
            in.data()));
}

void Gl_Image_Texture::copy(const V2i & in)
{
    //DJV_DEBUG("Gl_Image_Texture::copy");
    //DJV_DEBUG_PRINT("in = " << in);

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_2D, _id));

    DJV_DEBUG_GL(
        glCopyTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,
            0,
            0,
            0,
            in.x,
            in.y));
}

void Gl_Image_Texture::bind()
{
    //DJV_DEBUG("Gl_Image_Texture::bind");

    DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_2D, _id));
}

const Pixel_Data_Info & Gl_Image_Texture::info() const
{
    return _info;
}

GLenum Gl_Image_Texture::min() const
{
    return _min;
}

GLenum Gl_Image_Texture::mag() const
{
    return _mag;
}

GLuint Gl_Image_Texture::id() const
{
    return _id;
}

void Gl_Image_Texture::del()
{
    if (_id)
    {
        glDeleteTextures(1, &_id);

        _id = 0;
    }
}

//------------------------------------------------------------------------------
// Gl_Image_Shader
//------------------------------------------------------------------------------

class Gl_Image_Shader
{
public:

    Gl_Image_Shader();

    ~Gl_Image_Shader();

    void init(const String & vertex, const String & fragment) throw (Error);

    void bind();

    GLuint program() const;

private:

    void del();

    String _vertex;
    String _fragment;

    GLuint _vertex_id;
    GLuint _fragment_id;
    GLuint _program;
};

Gl_Image_Shader::Gl_Image_Shader() :
    _vertex_id  (0),
    _fragment_id(0),
    _program    (0)
{}

Gl_Image_Shader::~Gl_Image_Shader()
{
    del();
}

namespace
{

GLuint shaderCreate(GLenum type) throw (Error)
{
    //DJV_DEBUG("shaderCreate");

    GLuint r = 0;

    DJV_DEBUG_GL(r = glCreateShader(type));

    if (! r)
    {
        throw _ERROR("Cannot create shader");
    }

    return r;
}

void shader_compile(GLuint id, const String & source) throw (Error)
{
    //DJV_DEBUG("shader_compile");
    //DJV_DEBUG_PRINT("source = " << source);

    Memory_Buffer<char> buf;
    buf.size(source.size());
    Memory::copy(source.c_str(), buf.data(), buf.size());

    const char * sources        [] = { buf.data() };
    const GLint  source_lengths [] = { buf.size() };

    DJV_DEBUG_GL(glShaderSource(id, 1, sources, source_lengths));

    DJV_DEBUG_GL(glCompileShader(id));

    GLint error = GL_FALSE;

    glGetShaderiv(id, GL_COMPILE_STATUS, &error);

    char log [4096] = "";
    GLsizei log_size = 0;
    glGetShaderInfoLog(id, 4096, &log_size, log);

    //DJV_DEBUG_PRINT("log = " << String(log));

    if (error != GL_TRUE)
    {
        throw _ERROR(String_Format("Cannot compile shader:\n%%").arg(log));
    }
}

} // namespace

void Gl_Image_Shader::init(
    const String & vertex,
    const String & fragment) throw (Error)
{
    if (vertex == _vertex && fragment == _fragment)
    {
        return;
    }

    //DJV_DEBUG("Gl_Image_Shader::init");
    //DJV_DEBUG_PRINT("fragment = " << fragment);

    del();

    _vertex   = vertex;
    _fragment = fragment;

    GLint error = GL_FALSE;

    _vertex_id   = shaderCreate(GL_VERTEX_SHADER);
    _fragment_id = shaderCreate(GL_FRAGMENT_SHADER);

    shader_compile(_vertex_id,   _vertex);
    shader_compile(_fragment_id, _fragment);

    DJV_DEBUG_GL(_program = glCreateProgram());

    if (! _program)
    {
        throw _ERROR("Cannot create shader program");
    }

    DJV_DEBUG_GL(glAttachShader(_program, _vertex_id));
    DJV_DEBUG_GL(glAttachShader(_program, _fragment_id));
    DJV_DEBUG_GL(glLinkProgram (_program));

    glGetProgramiv(_program, GL_LINK_STATUS, &error);

    char log [4096] = "";
    GLsizei log_size = 0;
    glGetProgramInfoLog(_program, 4096, &log_size, log);

    //DJV_DEBUG_PRINT("log = " << String(log));

    if (error != GL_TRUE)
    {
        throw _ERROR(String_Format("Cannot link shader:\n%%").arg(log));
    }
}

void Gl_Image_Shader::bind()
{
    //DJV_DEBUG("Gl_Image_Shader::bind");

    DJV_DEBUG_GL(glUseProgram(_program));
}

GLuint Gl_Image_Shader::program() const
{
    return _program;
}

void Gl_Image_Shader::del()
{
    if (_vertex_id)
    {
        glDeleteShader(_vertex_id);
    }

    if (_fragment_id)
    {
        glDeleteShader(_fragment_id);
    }

    if (_program)
    {
        glDeleteProgram(_program);
    }
}

//------------------------------------------------------------------------------
// Gl_Image_State
//------------------------------------------------------------------------------

Gl_Image_State::Gl_Image_State() :
    _init               (false),
    _texture            (new Gl_Image_Texture),
    _shader             (new Gl_Image_Shader),
    _scale_x_contrib    (new Gl_Image_Texture),
    _scale_y_contrib    (new Gl_Image_Texture),
    _scale_x_shader     (new Gl_Image_Shader),
    _scale_y_shader     (new Gl_Image_Shader),
    _lut_color_profile  (new Gl_Image_Lut),
    _lut_display_profile(new Gl_Image_Lut)
{}

Gl_Image_State::~Gl_Image_State()
{
    delete _lut_display_profile;
    delete _lut_color_profile;
    delete _scale_x_shader;
    delete _scale_y_shader;
    delete _scale_x_contrib;
    delete _scale_y_contrib;
    delete _shader;
    delete _texture;
}

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------

namespace
{

void active_texture(GLenum in)
{
    //DJV_DEBUG("active_texture");

    DJV_DEBUG_GL(glActiveTexture(in));
}

void uniform1i(GLuint program, const String & name, int value)
{
    glUniform1i(
        glGetUniformLocation(program, name.c_str()),
        static_cast<GLint>(value));
}

void uniform1f(GLuint program, const String & name, double value)
{
    glUniform1f(
        glGetUniformLocation(program, name.c_str()),
        static_cast<GLfloat>(value));
}

void uniform_matrix4f(
    GLuint         program,
    const String & name,
    const M4f &    value)
{
    glUniformMatrix4fv(
        glGetUniformLocation(program, name.c_str()),
        1,
        false,
        Matrix_Util::convert<double, GLfloat>(Matrix_Util::transpose(value)).e);
}

typedef double (Filter_Fnc)(const double t);

static const double support_box = 0.5;

static double filter_box(double t)
{
    if (t > -0.5 && t <= 0.5)
    {
        return 1.0;
    }

    return 0.0;
}

static const double support_triangle = 1.0;

static double filter_triangle(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 1.0)
    {
        return 1.0 - t;
    }

    return 0.0;
}

static const double support_bell = 1.5;

static double filter_bell(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 0.5)
    {
        return 0.75 - t * t;
    }

    if (t < 1.5)
    {
        t = t - 1.5;
        return 0.5 * t * t;
    }

    return 0.0;
}

static const double support_bspline = 2.0;

static double filter_bspline(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 1.0)
    {
        const double tt = t * t;
        return (0.5 * tt * t) - tt + 2.0 / 3.0;
    }
    else if (t < 2.0)
    {
        t = 2.0 - t;
        return (1.0 / 6.0) * (t * t * t);
    }

    return 0.0;
}

static double sinc(double x)
{
    x *= Math::pi;

    if (x != 0.0)
    {
        return Math::sin(x) / x;
    }

    return 1.0;
}

static const double support_lanczos3 = 3.0;

static double filter_lanczos3(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 3.0)
    {
        return sinc(t) * sinc(t / 3.0);
    }

    return 0.0;
}

static const double support_cubic = 1.0;

static double filter_cubic(double t)
{
    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 1.0)
    {
        return (2.0 * t - 3.0) * t * t + 1.0;
    }

    return 0.0;
}

static const double support_mitchell = 2.0;

static double filter_mitchell(double t)
{
    const double tt = t * t;
    static const double b = 1.0 / 3.0;
    static const double c = 1.0 / 3.0;

    if (t < 0.0)
    {
        t = -t;
    }

    if (t < 1.0)
    {
        t =
            ((12.0 - 9.0 * b - 6.0 * c) * (t * tt)) +
            ((-18.0 + 12.0 * b + 6.0 * c) * tt) +
            (6.0 - 2.0 * b);
        return t / 6.0;
    }
    else if (t < 2.0)
    {
        t =
            ((-1.0 * b - 6.0 * c) * (t * tt)) +
            ((6.0 * b + 30.0 * c) * tt) +
            ((-12.0 * b - 48.0 * c) * t) +
            (8.0 * b + 24.0 * c);
        return t / 6.0;
    }

    return 0.0;
}

Filter_Fnc * filter_fnc(Gl_Image_Filter::FILTER in)
{
    static Filter_Fnc * tmp [] =
    {
        filter_box,
        filter_box,
        filter_box,
        filter_triangle,
        filter_bell,
        filter_bspline,
        filter_lanczos3,
        filter_cubic,
        filter_mitchell
    };

    return tmp[in];
}

static double filter_support(Gl_Image_Filter::FILTER in)
{
    static const double tmp [] =
    {
        support_box,
        support_box,
        support_box,
        support_triangle,
        support_bell,
        support_bspline,
        support_lanczos3,
        support_cubic,
        support_mitchell
    };

    return tmp[in];
}

int edge(int in, int size)
{
    return Math::clamp(in, 0, size - 1);
}

void scale_contrib(
    int                     input,
    int                     output,
    Gl_Image_Filter::FILTER filter,
    Pixel_Data &            data)
{
    //DJV_DEBUG("scale_contrib");
    //DJV_DEBUG_PRINT("scale = " << input << " " << output);
    //DJV_DEBUG_PRINT("filter = " << filter);

    // Filter function.

    Filter_Fnc * fnc = filter_fnc(filter);

    const double support = filter_support(filter);

    //DJV_DEBUG_PRINT("support = " << support);

    const double scale =
        static_cast<double>(output) / static_cast<double>(input);

    //DJV_DEBUG_PRINT("scale = " << scale);

    const double radius =
        support * (scale >= 1.0 ? 1.0 : (1.0 / scale));

    //DJV_DEBUG_PRINT("radius = " << radius);

    // Initialize.

    const int width = Math::ceil(radius * 2 + 1);

    //DJV_DEBUG_PRINT("width = " << width);

    data.set(Pixel_Data_Info(V2i(output, width), Pixel::LA_F32));

    // Work.

    for (int i = 0; i < output; ++i)
    {
        const double center = i / scale;
        const int    left   = Math::ceil(center - radius);
        const int    right  = Math::floor(center + radius);

        //DJV_DEBUG_PRINT(i << " = " << left << " " << center << " " << right);

        double sum   = 0.0;
        int    pixel = 0;

        int j = 0;

        for (int k = left; j < width && k <= right; ++j, ++k)
        {
            Pixel::F32_T * p =
                reinterpret_cast<Pixel::F32_T *>(data.data(i, j));

            pixel = edge(k, input);

            const double x = (center - k) * (scale < 1.0 ? scale : 1.0);
            const double w = (scale < 1.0) ? ((*fnc)(x) * scale) : (*fnc)(x);

            //DJV_DEBUG_PRINT("w = " << w);

            p[0] = static_cast<Pixel::F32_T>(pixel / double(input));
            p[1] = static_cast<Pixel::F32_T>(w);

            sum += w;
        }

        for (; j < width; ++j)
        {
            Pixel::F32_T * p =
                reinterpret_cast<Pixel::F32_T *>(data.data(i, j));

            p[0] = static_cast<Pixel::F32_T>(pixel / double(input));
            p[1] = 0.0f;
        }

        /*for (j = 0; j < width; ++j)
        {
          Pixel::F32_T * p =
            reinterpret_cast<Pixel::F32_T *>(data.data(i, j));

          //DJV_DEBUG_PRINT(p[0] << " = " << p[1]);
        }
        //DJV_DEBUG_PRINT("sum = " << sum);*/

        //! \todo Why do we have to average these?

        for (j = 0; j < width; ++j)
        {
            Pixel::F32_T * p =
                reinterpret_cast<Pixel::F32_T *>(data.data(i, j));

            p[1] /= static_cast<Pixel::F32_T>(sum);
        }
    }
}

void quad(
    const V2i & size,
    const V2b & mirror      = V2b(),
    int         proxy_scale = 1)
{
    //DJV_DEBUG("quad");

    double u [] = { 0, 0 };
    double v [] = { 0, 0 };

    u[! mirror.x] = 1.0;
    v[! mirror.y] = 1.0;

    //DJV_DEBUG_PRINT("u = " << u[0] << " " << u[1]);
    //DJV_DEBUG_PRINT("v = " << v[0] << " " << v[1]);

    const V2f uv [] =
    {
        V2f(u[0], v[0]),
        V2f(u[0], v[1]),
        V2f(u[1], v[1]),
        V2f(u[1], v[0])
    };

    glBegin(GL_QUADS);

    Gl_Util::draw_box(size * proxy_scale, uv);

    glEnd();
}

} // namespace

//------------------------------------------------------------------------------
// Shader Source
//------------------------------------------------------------------------------

namespace
{

const String source_vertex =
    "void main(void)\n"
    "{\n"
    "  gl_FrontColor  = gl_Color;\n"
    "  gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "  gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "}\n";

const String source_fragment_header =
    "struct Levels\n"
    "{\n"
    "  float in0, in1;\n"
    "  float gamma;\n"
    "  float out0, out1;\n"
    "};\n"
    "\n"
    "struct Exposure\n"
    "{\n"
    "  float v, d, k, f, g;\n"
    "};\n"
    "\n"
    "float knee(float value, float f)\n"
    "{\n"
    "  return log(value * f + 1.0) / f;\n"
    "}\n"
    "\n"
    "vec4 lut(vec4 value, sampler1D lut)\n"
    "{\n"
    "  value[0] = texture1D(lut, value[0])[0];\n"
    "  value[1] = texture1D(lut, value[1])[1];\n"
    "  value[2] = texture1D(lut, value[2])[2];\n"
    "\n"
    "    return value;\n"
    "}\n"
    "\n"
    "vec4 gamma(vec4 value, float gamma)\n"
    "{\n"
    "  value[0] = pow(value[0], 1.0 / gamma);\n"
    "  value[1] = pow(value[1], 1.0 / gamma);\n"
    "  value[2] = pow(value[2], 1.0 / gamma);\n"
    "\n"
    "  return value;\n"
    "}\n"
    "\n"
    "vec4 display_profile_color(vec4 value, mat4 color)\n"
    "{\n"
    "  vec4 tmp;\n"
    "\n"
    "  tmp[0] = value[0];\n"
    "  tmp[1] = value[1];\n"
    "  tmp[2] = value[2];\n"
    "  tmp[3] = 1.0;\n"
    "\n"
    "  tmp *= color;\n"
    "\n"
    "  tmp[3] = value[3];\n"
    "\n"
    "  return tmp;\n"
    "}\n"
    "\n"
    "vec4 levels(vec4 value, Levels data)\n"
    "{\n"
    "  value[0] = pow(max(value[0] - data.in0, 0.0) / data.in1, data.gamma) *\n"
    "      data.out1 + data.out0;\n"
    "\n"
    "  value[1] = pow(max(value[1] - data.in0, 0.0) / data.in1, data.gamma) *\n"
    "      data.out1 + data.out0;\n"
    "\n"
    "  value[2] = pow(max(value[2] - data.in0, 0.0) / data.in1, data.gamma) *\n"
    "       data.out1 + data.out0;\n"
    "\n"
    "    return value;\n"
    "}\n"
    "\n"
    "vec4 exposure(vec4 value, Exposure data)\n"
    "{\n"
    "  value[0] = max(0.0, value[0] - data.d) * data.v;\n"
    "  value[1] = max(0.0, value[1] - data.d) * data.v;\n"
    "  value[2] = max(0.0, value[2] - data.d) * data.v;\n"
    "\n"
    "  if (value[0] > data.k)\n"
    "    value[0] = data.k + knee(value[0] - data.k, data.f);\n"
    "  if (value[1] > data.k)\n"
    "    value[1] = data.k + knee(value[1] - data.k, data.f);\n"
    "  if (value[2] > data.k)\n"
    "    value[2] = data.k + knee(value[2] - data.k, data.f);\n"
    "\n"
    "  value[0] *= 0.332;\n"
    "  value[1] *= 0.332;\n"
    "  value[2] *= 0.332;\n"
    "\n"
    "  return value;\n"
    "}\n"
    "\n"
    "vec4 soft_clip(vec4 value, float soft_clip)\n"
    "{\n"
    "  float tmp = 1.0 - soft_clip;\n"
    "\n"
    "  if (value[0] > tmp)\n"
    "    value[0] = tmp + (1.0 - exp(-(value[0] - tmp) / soft_clip)) *\n"
    "      soft_clip;\n"
    "\n"
    "  if (value[1] > tmp)\n"
    "    value[1] = tmp + (1.0 - exp(-(value[1] - tmp) / soft_clip)) *\n"
    "      soft_clip;\n"
    "\n"
    "  if (value[2] > tmp)\n"
    "    value[2] = tmp + (1.0 - exp(-(value[2] - tmp) / soft_clip)) *\n"
    "      soft_clip;\n"
    "\n"
    "  return value;\n"
    "}\n"
    "\n";

const String source_fragment_scale_x =
    "vec4 scale_x(sampler2D texture, sampler2D contrib)\n"
    "{\n"
    "  vec4 value = vec4(0.0);\n"
    "\n"
    "  for (int i = 0; i < %%; ++i)\n"
    "  {\n"
    "    float t = float(i) / float(%% - 1);\n"
    "\n"
    "    vec4 tmp = texture2D(contrib, vec2(gl_TexCoord[0].s, t));\n"
    "\n"
    "    vec2 position = vec2(tmp[0], gl_TexCoord[0].t);\n"
    "\n"
    "    value += tmp[3] * %%;\n"
    "  }\n"
    "\n"
    "  return value;\n"
    "}\n"
    "\n";

const String source_fragment_scale_y =
    "vec4 scale_y(sampler2D texture, sampler2D contrib)\n"
    "{\n"
    "  vec4 value = vec4(0.0);\n"
    "\n"
    "  for (int i = 0; i < %%; ++i)\n"
    "  {\n"
    "    float t = float(i) / float(%% - 1);\n"
    "\n"
    "    vec4 tmp = texture2D(contrib, vec2(gl_TexCoord[0].t, t));\n"
    "\n"
    "    vec2 position = vec2(gl_TexCoord[0].s, tmp[0]);\n"
    "\n"
    "    value += tmp[3] * texture2D(texture, position);\n"
    "  }\n"
    "\n"
    "  return value;\n"
    "}\n"
    "\n";

const String source_fragment_main =
    "void main(void)\n"
    "{\n"
    "  vec4 color;\n"
    "\n"
    "%%"
    "\n"
    "  gl_FragColor = color;\n"
    "}\n";

} // namespace

//------------------------------------------------------------------------------
// source_fragment
//
// Generate fragment shader source code.
//------------------------------------------------------------------------------

namespace
{

/*String source_fragment()
{
  String header;
  String main;

  header = "uniform sampler2D inTexture;\n";

  main +=
    "vec2 position = gl_TexCoord[0].st;\n";
  main +=
    "color = texture2D(inTexture, position);\n";

  return
    header + "\n" +
    String(source_fragment_main).arg(main);
}*/

String source_fragment(
    Color_Profile::PROFILE           color_profile,
    const Gl_Image_Display_Profile & display_profile,
    Gl_Image_Options::CHANNEL        channel,
    bool                             multipass_filter,
    int                              scale_size,
    bool                             scale_x)
{
    //DJV_DEBUG("source_fragment");
    //DJV_DEBUG_PRINT("color_profile = " << color_profile);
    //DJV_DEBUG_PRINT("display_profile = " << display_profile);
    //DJV_DEBUG_PRINT("channel = " << channel);
    //DJV_DEBUG_PRINT("multipass_filter = " << multipass_filter);
    //DJV_DEBUG_PRINT("scale_size = " << scale_size);
    //DJV_DEBUG_PRINT("scale_x = " << scale_x);

    String header;
    String main;

    // Initialize header.

    header = source_fragment_header;
    header += "uniform sampler2D inTexture;\n";

    // Color profile.

    String sample;

    switch (color_profile)
    {
        case Color_Profile::LUT:
            header +=
                "uniform sampler1D inColorProfileLut;\n";
            sample =
                "lut(\n"
                "    texture2D(inTexture, position),\n"
                "    inColorProfileLut)";
            break;

        case Color_Profile::GAMMA:
            header +=
                "uniform float inColorProfileGamma;\n";
            sample =
                "gamma(\n"
                "    texture2D(inTexture, position),\n"
                "    inColorProfileGamma)";
            break;

        case Color_Profile::EXPOSURE:
            header +=
                "uniform Exposure inColorProfileExposure;\n";
            sample =
                "exposure(\n"
                "    texture2D(inTexture, position),\n"
                "    inColorProfileExposure)";
            break;

        default:
            sample =
                "texture2D(inTexture, position)";
            break;
    }

    // Image filter.

    if (! multipass_filter)
    {
        main +=
            "vec2 position = gl_TexCoord[0].st;\n";
        main +=
            String_Format("color = %%;\n").arg(sample);
    }
    else
    {
        header +=
            "uniform sampler2D inScaleContrib;\n";

        if (scale_x)
        {
            header += String_Format(source_fragment_scale_x).
                      arg(scale_size).
                      arg(scale_size).
                      arg(sample);
            main += "color = scale_x(inTexture, inScaleContrib);\n";
        }
        else
        {
            header += String_Format(source_fragment_scale_y).
                      arg(scale_size).
                      arg(scale_size);
            main += "color = scale_y(inTexture, inScaleContrib);\n";
        }
    }

    // Display profile.

    if (Vector_Util::is_size_valid(display_profile.lut.size()))
    {
        header +=
            "uniform sampler1D inDisplayProfileLut;\n";
        main +=
            "color = lut(color, inDisplayProfileLut);\n";
    }

    if (display_profile.color != Gl_Image_Display_Profile().color)
    {
        header +=
            "uniform mat4 inDisplayProfileColor;\n";
        main +=
            "color = display_profile_color(color, inDisplayProfileColor);\n";
    }

    if (display_profile.levels != Gl_Image_Display_Profile().levels)
    {
        header +=
            "uniform Levels inDisplayProfileLevels;\n";
        main +=
            "color = levels(color, inDisplayProfileLevels);\n";
    }

    if (display_profile.soft_clip != Gl_Image_Display_Profile().soft_clip)
    {
        header +=
            "uniform float inDisplayProfileSoftClip;\n";
        main +=
            "color = soft_clip(color, inDisplayProfileSoftClip);\n";
    }

    // Image channel.

    if (channel)
    {
        main += String_Format("color = vec4(color[%%]);\n").arg(channel - 1);
    }

    // Clamp pixel values.

    //if (clamp)
    //  main += "color = clamp(color, vec4(0.0), vec4(1.0));\n";

    return
        header + "\n" +
        String(String_Format(source_fragment_main).arg(main));
}

} // namespace

//------------------------------------------------------------------------------
// color_profile_init
//
// Initialize color profile.
//------------------------------------------------------------------------------

namespace
{

double knee(double x, double f)
{
    return Math::log(x * f + 1.0) / f;
}

double knee2(double x, double y)
{
    double f0 = 0.0, f1 = 1.0;

    while (knee(x, f1) > y)
    {
        f0 = f1;
        f1 = f1 * 2.0;
    }

    for (int i = 0; i < 30; ++i)
    {
        const double f2 = (f0 + f1) / 2.0;

        if (knee(x, f2) < y)
        {
            f1 = f2;
        }
        else
        {
            f0 = f2;
        }
    }

    return (f0 + f1) / 2.0;
}

void color_profile_init(
    const Gl_Image_Options & options,
    GLuint                   program,
    Gl_Image_Lut &           color_profile)
{
    //DJV_DEBUG("color_profile_init");

    switch (options.color_profile.type)
    {
        case Color_Profile::LUT:
        {
            active_texture(GL_TEXTURE2);

            uniform1i(program, "inColorProfileLut", 2);

            color_profile.init(options.color_profile.lut);
        }
        break;

        case Color_Profile::GAMMA:
        {
            uniform1f(
                program,
                "inColorProfileGamma",
                options.color_profile.gamma);
        }
        break;

        case Color_Profile::EXPOSURE:
        {
            struct Exposure
            {
                double v, d, k, f;
            }
            exposure;

            exposure.v = Math::pow(
                2.0,
                options.color_profile.exposure.value + 2.47393);
            exposure.d = options.color_profile.exposure.defog;
            exposure.k = Math::pow(
                2.0,
                options.color_profile.exposure.knee_low);
            exposure.f = knee2(
                Math::pow(2.0, options.color_profile.exposure.knee_high) -
                exposure.k,
                Math::pow(2.0, 3.5) - exposure.k);

            //DJV_DEBUG_PRINT("exposure");
            //DJV_DEBUG_PRINT("  v = " << exposure.v);
            //DJV_DEBUG_PRINT("  d = " << exposure.d);
            //DJV_DEBUG_PRINT("  k = " << exposure.k);
            //DJV_DEBUG_PRINT("  f = " << exposure.f);

            uniform1f(program, "inColorProfileExposure.v", exposure.v);
            uniform1f(program, "inColorProfileExposure.d", exposure.d);
            uniform1f(program, "inColorProfileExposure.k", exposure.k);
            uniform1f(program, "inColorProfileExposure.f", exposure.f);
        }
        break;

        default:
            break;
    }
}

} // namespace

//------------------------------------------------------------------------------
// display_profile_init
//
// Initialize display profile.
//------------------------------------------------------------------------------

namespace
{

void display_profile_init(
    const Gl_Image_Options & options,
    GLuint                   program,
    Gl_Image_Lut &           display_profile)
{
    //DJV_DEBUG("display_profile_init");

    // Color matrix.

    uniform_matrix4f(
        program,
        "inDisplayProfileColor",
        Gl_Image_Color::color_matrix(options.display_profile.color));

    // Levels in.

    uniform1f(
        program,
        "inDisplayProfileLevels.in0",
        options.display_profile.levels.in_low);

    uniform1f(
        program,
        "inDisplayProfileLevels.in1",
        options.display_profile.levels.in_high -
            options.display_profile.levels.in_low);

    // Gamma.

    uniform1f(
        program,
        "inDisplayProfileLevels.gamma",
        1.0 / options.display_profile.levels.gamma);

    // Levels out.

    uniform1f(
        program,
        "inDisplayProfileLevels.out0",
        options.display_profile.levels.out_low);

    uniform1f(
        program,
        "inDisplayProfileLevels.out1",
        options.display_profile.levels.out_high -
            options.display_profile.levels.out_low);

    // Soft-clip.

    uniform1f(
        program,
        "inDisplayProfileSoftClip",
        options.display_profile.soft_clip);

    // Lookup table.

    if (Vector_Util::is_size_valid(options.display_profile.lut.size()))
    {
        active_texture(GL_TEXTURE3);
        uniform1i(program, "inDisplayProfileLinDisplayProfileut", 3);
        display_profile.init(options.display_profile.lut);
    }
}

} // namespace

//------------------------------------------------------------------------------
// Gl_Image::draw
//------------------------------------------------------------------------------

namespace
{

struct Restore_State
{
    ~Restore_State()
    {
        glUseProgram(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

} // namespace

void Gl_Image::draw(
    const Pixel_Data &       data,
    const Gl_Image_Options & options,
    Gl_Image_State *         state) throw (Error)
{
    //DJV_DEBUG("Gl_Image::draw");
    //DJV_DEBUG_PRINT("data = " << data);
    //DJV_DEBUG_PRINT("color profile = " << options.color_profile);

    Restore_State restore_state;

    Gl_Image_State default_state;

    if (! state)
    {
        state = &default_state;
    }

    const Pixel_Data_Info & info = data.info();

    const int proxy_scale =
        options.proxy_scale ?
        Pixel_Data::proxy_scale(info.proxy) :
        1;

    const V2i scale = Vector_Util::ceil<double, int>(
        options.xform.scale * V2f(info.size * proxy_scale));

    const V2i scale_tmp(scale.x, data.h());

    //DJV_DEBUG_PRINT("scale = " << scale);
    //DJV_DEBUG_PRINT("scale_tmp = " << scale_tmp);

    // Initialize.

    const Gl_Image_Filter::FILTER filter =
        info.size == scale ? Gl_Image_Filter::NEAREST :
        (Vector_Util::area(scale) < Vector_Util::area(info.size) ?
         options.filter.min : options.filter.mag);

    //DJV_DEBUG_PRINT("filter min = " << options.filter_options.min);
    //DJV_DEBUG_PRINT("filter mag = " << options.filter_options.mag);
    //DJV_DEBUG_PRINT("filter = " << filter);

    if (! state->_init || state->_info != info || state->_options != options)
    {
        switch (filter)
        {
            case Gl_Image_Filter::NEAREST:
            case Gl_Image_Filter::LINEAR:
            {
                //DJV_DEBUG_PRINT("init single pass");

                state->_texture->init(
                    data.info(),
                    Gl_Image_Filter::filter_to_gl(filter),
                    Gl_Image_Filter::filter_to_gl(filter));

                state->_shader->init(
                    source_vertex,
                    source_fragment(
                        options.color_profile.type,
                        options.display_profile,
                        options.channel,
                        false,
                        0,
                        false));
            }
            break;

            case Gl_Image_Filter::BOX:
            case Gl_Image_Filter::TRIANGLE:
            case Gl_Image_Filter::BELL:
            case Gl_Image_Filter::BSPLINE:
            case Gl_Image_Filter::LANCZOS3:
            case Gl_Image_Filter::CUBIC:
            case Gl_Image_Filter::MITCHELL:
            {
                //DJV_DEBUG_PRINT("init two pass");

                state->_texture->init(
                    data.info(),
                    GL_NEAREST,
                    GL_NEAREST);

                // Initialize horizontal pass.

                Pixel_Data contrib;

                scale_contrib(
                    data.w(),
                    scale.x,
                    filter,
                    contrib);

                state->_scale_x_contrib->init(
                    contrib,
                    GL_NEAREST,
                    GL_NEAREST);

                state->_scale_x_shader->init(
                    source_vertex,
                    source_fragment(
                        options.color_profile.type,
                        Gl_Image_Display_Profile(),
                        static_cast<Gl_Image_Options::CHANNEL>(0),
                        true,
                        contrib.h(),
                        true));

                // Initialize vertical pass.

                scale_contrib(
                    data.h(),
                    scale.y,
                    filter,
                    contrib);

                state->_scale_y_contrib->init(
                    contrib,
                    GL_NEAREST,
                    GL_NEAREST);

                state->_scale_y_shader->init(
                    source_vertex,
                    source_fragment(
                        static_cast<Color_Profile::PROFILE>(0),
                        options.display_profile,
                        options.channel,
                        true,
                        contrib.h(),
                        false));
            }
            break;

            default:
                break;
        }

        state->_init    = true;
        state->_info    = info;
        state->_options = options;
    }

    // Render.

    const V2b mirror(
        info.mirror.x ? (! options.xform.mirror.x) : options.xform.mirror.x,
        info.mirror.y ? (! options.xform.mirror.y) : options.xform.mirror.y);

    //DJV_DEBUG_PRINT("mirror = " << mirror);

    switch (filter)
    {
        case Gl_Image_Filter::NEAREST:
        case Gl_Image_Filter::LINEAR:
        {
            //DJV_DEBUG_PRINT("draw single pass");

            state->_shader->bind();

            // Initialize color and display profiles.

            color_profile_init(
                options,
                state->_shader->program(),
                *state->_lut_color_profile);

            display_profile_init(
                options,
                state->_shader->program(),
                *state->_lut_display_profile);

            // Draw.

            active_texture(GL_TEXTURE0);

            uniform1i(state->_shader->program(), "inTexture", 0);

            state->_texture->copy(data);
            state->_texture->bind();

            DJV_DEBUG_GL(glPushMatrix());
            const M3f m = Gl_Image_Xform::xform_matrix(options.xform);
            //DJV_DEBUG_PRINT("m = " << m);
            DJV_DEBUG_GL(glLoadMatrixd(Matrix_Util::matrix4(m).e));

            quad(info.size, mirror, proxy_scale);

            DJV_DEBUG_GL(glPopMatrix());
        }
        break;

        case Gl_Image_Filter::BOX:
        case Gl_Image_Filter::TRIANGLE:
        case Gl_Image_Filter::BELL:
        case Gl_Image_Filter::BSPLINE:
        case Gl_Image_Filter::LANCZOS3:
        case Gl_Image_Filter::CUBIC:
        case Gl_Image_Filter::MITCHELL:
        {
            //DJV_DEBUG_PRINT("draw two pass");

            // Horizontal pass.

            Gl_Offscreen_Buffer buffer(
                Pixel_Data_Info(scale_tmp, data.pixel()));

            {
                Gl_Offscreen_Buffer_Scope bufferScope(&buffer);

                state->_scale_x_shader->bind();

                color_profile_init(
                    options,
                    state->_scale_x_shader->program(),
                    *state->_lut_color_profile);

                active_texture(GL_TEXTURE0);

                uniform1i(state->_scale_x_shader->program(), "inTexture", 0);

                state->_texture->copy(data);
                state->_texture->bind();

                active_texture(GL_TEXTURE1);

                uniform1i(
                    state->_scale_x_shader->program(), "inScaleContrib", 1);

                state->_scale_x_contrib->bind();

                glPushAttrib(GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);
                glMatrixMode(GL_PROJECTION);
                glPushMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();

                Gl_Util::ortho(scale_tmp);
                glViewport(0, 0, scale_tmp.x, scale_tmp.y);
                quad(scale_tmp, mirror);

                glMatrixMode(GL_PROJECTION);
                glPopMatrix();
                glMatrixMode(GL_MODELVIEW);
                glPopMatrix();
                glPopAttrib();
            }

            // Vertical pass.

            state->_scale_y_shader->bind();

            display_profile_init(
                options,
                state->_scale_y_shader->program(),
                *state->_lut_display_profile);

            active_texture(GL_TEXTURE0);

            uniform1i(state->_scale_y_shader->program(), "inTexture", 0);

            DJV_DEBUG_GL(glBindTexture(GL_TEXTURE_2D, buffer.texture()));

            active_texture(GL_TEXTURE1);

            uniform1i(state->_scale_y_shader->program(), "inScaleContrib", 1);

            state->_scale_y_contrib->bind();

            Gl_Image_Xform xform = options.xform;
            xform.scale = V2f(1.0);
            const M3f m = Gl_Image_Xform::xform_matrix(xform);

            DJV_DEBUG_GL(glPushMatrix());
            DJV_DEBUG_GL(glLoadMatrixd(Matrix_Util::matrix4(m).e));

            quad(scale);

            DJV_DEBUG_GL(glPopMatrix());
        }
        break;

        default:
            break;
    }
}

} // djv
