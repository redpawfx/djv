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

//! \file djv_gl_image.cpp

#include <djv_gl_image.h>

#include <djv_gl_offscreen_buffer.h>

namespace djv
{

//------------------------------------------------------------------------------
// Gl_Image_Xform
//------------------------------------------------------------------------------

Gl_Image_Xform::Gl_Image_Xform() :
    scale (1.0),
    rotate(0.0)
{}

M3f Gl_Image_Xform::xform_matrix(const Gl_Image_Xform & in)
{
    return
        Matrix_Util::scale3f(in.scale) *
        Matrix_Util::rotate3f(in.rotate) *
        Matrix_Util::translate3f(in.position);
}

//------------------------------------------------------------------------------
// Gl_Image_Color
//------------------------------------------------------------------------------

Gl_Image_Color::Gl_Image_Color() :
    brightness(1.0),
    contrast  (1.0),
    saturation(1.0)
{}

M4f Gl_Image_Color::brightness_matrix(double r, double g, double b)
{
    return M4f(
        r, 0, 0, 0,
        0, g, 0, 0,
        0, 0, b, 0,
        0, 0, 0, 1);
}

M4f Gl_Image_Color::contrast_matrix(double r, double g, double b)
{
    return M4f(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        -0.5, -0.5, -0.5, 1) *
        M4f(
            r, 0, 0, 0,
            0, g, 0, 0,
            0, 0, b, 0,
            0, 0, 0, 1) *
        M4f(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0.5, 0.5, 0.5, 1);
}

M4f Gl_Image_Color::saturation_matrix(double r, double g, double b)
{
    const double s [] =
    {
        (1.0 - r) * 0.3086,
        (1.0 - g) * 0.6094,
        (1.0 - b) * 0.0820
    };

    return M4f(
        s[0] + r, s[0],     s[0],     0,
        s[1],     s[1] + g, s[1],     0,
        s[2],     s[2],     s[2] + b, 0,
        0,        0,        0,        1);
}

M4f Gl_Image_Color::color_matrix(const Gl_Image_Color & in)
{
    return
        brightness_matrix(in.brightness, in.brightness, in.brightness) *
        contrast_matrix(in.contrast, in.contrast, in.contrast) *
        saturation_matrix(in.saturation, in.saturation, in.saturation);
}

//------------------------------------------------------------------------------
// Gl_Image_Levels
//------------------------------------------------------------------------------

Gl_Image_Levels::Gl_Image_Levels() :
    in_low  (0.0),
    in_high (1.0),
    gamma   (1.0),
    out_low (0.0),
    out_high(1.0)
{}

Pixel_Data Gl_Image_Levels::color_lut(
    const Gl_Image_Levels & in,
    double soft_clip)
{
    Pixel_Data out(Pixel_Data_Info(V2i(1024, 1), Pixel::L_F32));

    const double in_tmp = in.in_high - in.in_low;
    const double gamma = 1.0 / in.gamma;
    const double out_tmp = in.out_high - in.out_low;

    Pixel::F32_T * p = reinterpret_cast<Pixel::F32_T *>(out.data());
    const int size = out.size().x;

    for (int i = 0; i < size; ++i, ++p)
        *p = static_cast<Pixel::F32_T>(
            Math::soft_clip(
                Math::pow(
                    Math::max(
                        ((i / static_cast<double>(size - 1) - in.in_low) /
                            in_tmp),
                        0.000001),
                    gamma) * out_tmp + in.out_low,
                soft_clip));

    return out;
}

//------------------------------------------------------------------------------
// Gl_Image_Display_Profile
//------------------------------------------------------------------------------

Gl_Image_Display_Profile::Gl_Image_Display_Profile() :
    soft_clip(0.0)
{}

//------------------------------------------------------------------------------
// Gl_Image_Filter
//------------------------------------------------------------------------------

const List<String> & Gl_Image_Filter::label_filter()
{
    static const List<String> data = List<String>() <<
        "Nearest" <<
        "Linear" <<
        "Box" <<
        "Triangle" <<
        "Bell" <<
        "B-Spline" <<
        "Lanczos3" <<
        "Cubic" <<
        "Mitchell";

    DJV_ASSERT(data.size() == _FILTER_SIZE);

    return data;
}

Gl_Image_Filter::Gl_Image_Filter() :
    min(default_filter.min),
    mag(default_filter.mag)
{}

Gl_Image_Filter::Gl_Image_Filter(FILTER min, FILTER mag) :
    min(min),
    mag(mag)
{}

GLuint Gl_Image_Filter::filter_to_gl(FILTER in)
{
    switch (in)
    {
        case NEAREST:
            return GL_NEAREST;

        case LINEAR:
            return GL_LINEAR;

        default:
            break;
    }

    return 0;
}

Gl_Image_Filter Gl_Image_Filter::default_filter =
    Gl_Image_Filter(LINEAR, NEAREST);

//------------------------------------------------------------------------------
// Gl_Image_Options
//------------------------------------------------------------------------------

Gl_Image_Options::Gl_Image_Options() :
    channel    (CHANNEL_DEFAULT),
    proxy_scale(true)
{}

const List<String> & Gl_Image_Options::label_channel()
{
    static const List<String> data = List<String>() <<
        "Default" <<
        "Red" <<
        "Green" <<
        "Blue" <<
        "Alpha";

    DJV_ASSERT(data.size() == _CHANNEL_SIZE);

    return data;
}

//------------------------------------------------------------------------------
// Gl_Image
//------------------------------------------------------------------------------

void Gl_Image::read(Pixel_Data & output)
{
    read(output, Box2i(output.size()));
}

void Gl_Image::read(Pixel_Data & output, const Box2i & area)
{
    //DJV_DEBUG("Gl_Image::read");
    //DJV_DEBUG_PRINT("output = " << output);
    //DJV_DEBUG_PRINT("area = " << area);

    const Pixel_Data_Info & info = output.info();

    DJV_DEBUG_GL(glPushAttrib(
                     GL_CURRENT_BIT |
                     GL_ENABLE_BIT |
                     GL_PIXEL_MODE_BIT));
    DJV_DEBUG_GL(glDisable(GL_DITHER));

    //! \todo OpenGL sums color components for luminance?

    switch (Pixel::format(info.pixel))
    {
        case Pixel::L:
        case Pixel::LA:

            DJV_DEBUG_GL(glPixelTransferf(GL_GREEN_SCALE, 0.0));
            DJV_DEBUG_GL(glPixelTransferf(GL_BLUE_SCALE, 0.0));

            DJV_DEBUG_GL(glPixelTransferf(GL_RED_SCALE,
                static_cast<GLfloat>(1.0 / 3.0)));
            DJV_DEBUG_GL(glPixelTransferf(GL_GREEN_SCALE,
                static_cast<GLfloat>(1.0 / 3.0)));
            DJV_DEBUG_GL(glPixelTransferf(GL_BLUE_SCALE,
                static_cast<GLfloat>(1.0 / 3.0)));

            break;

        default:
            break;
    }

    state_pack(info, area.position);

    DJV_DEBUG_GL(glReadPixels(
        0, 0, area.w, area.h,
        Gl_Util::format(info.pixel, info.bgr),
        Gl_Util::type(info.pixel),
        output.data()));

    //stateReset();

    DJV_DEBUG_GL(glPopAttrib());
}

void Gl_Image::copy(
    const Pixel_Data &       input,
    Pixel_Data &             output,
    const Gl_Image_Options & options,
    Gl_Image_State *         state,
    Gl_Offscreen_Buffer *    buffer) throw (Error)
{
    //DJV_DEBUG("Gl_Image::copy");
    //DJV_DEBUG_PRINT("input = " << input);
    //DJV_DEBUG_PRINT("output = " << output);
    //DJV_DEBUG_PRINT("scale = " << options.xform.scale);

    const V2i & size = output.info().size;

    std::auto_ptr<Gl_Offscreen_Buffer> _buffer;

    if (! buffer)
    {
        //DJV_DEBUG_PRINT("create buffer");

        _buffer = std::auto_ptr<Gl_Offscreen_Buffer>(
            new Gl_Offscreen_Buffer(Pixel_Data_Info(size, output.pixel())));

        buffer = _buffer.get();
    }

    try
    {
        Gl_Offscreen_Buffer_Scope buffer_scope(buffer);

        Gl_Util::ortho(size);

        DJV_DEBUG_GL(glViewport(0, 0, size.x, size.y));

        //DJV_DEBUG_GL(glClearColor(0, 1, 0, 0));

        Color background(Pixel::RGB_F32);

        Color::convert(options.background, background);

        DJV_DEBUG_GL(glClearColor(
            background.get_f32(0),
            background.get_f32(1),
            background.get_f32(2),
            0.0));

        DJV_DEBUG_GL(glClear(GL_COLOR_BUFFER_BIT));

        Gl_Image_Options _options = options;

        if (output.info().mirror.x)
        {
            _options.xform.mirror.x = ! _options.xform.mirror.x;
        }

        if (output.info().mirror.y)
        {
            _options.xform.mirror.y = ! _options.xform.mirror.y;
        }

        draw(input, _options, state);

        read(output);
    }
    catch (const Error & error)
    {
        throw error;
    }
}

void Gl_Image::state_unpack(const Pixel_Data_Info & in, const V2i & offset)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, in.align);
    glPixelStorei(GL_UNPACK_SWAP_BYTES, in.endian != Memory::endian());
    //glPixelStorei(GL_UNPACK_ROW_LENGTH, in.data_window.w);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, offset.y);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, offset.x);
}

void Gl_Image::state_pack(const Pixel_Data_Info & in, const V2i & offset)
{
    glPixelStorei(GL_PACK_ALIGNMENT, in.align);
    glPixelStorei(GL_PACK_SWAP_BYTES, in.endian != Memory::endian());
    glPixelStorei(GL_PACK_ROW_LENGTH, in.size.x);
    glPixelStorei(GL_PACK_SKIP_ROWS, offset.y);
    glPixelStorei(GL_PACK_SKIP_PIXELS, offset.x);
}

void Gl_Image::state_reset()
{
    state_pack(Pixel_Data_Info());
    state_unpack(Pixel_Data_Info());

    if (GLEW_ARB_shader_objects)
    {
        glUseProgramObjectARB(0);
    }

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void Gl_Image::average(
    const Pixel_Data & in,
    Color *            out) throw (Error)
{

    DJV_ASSERT(out);

    //DJV_DEBUG("average");
    //DJV_DEBUG_PRINT("in = " << in);

    out->pixel(in.pixel());

    const Pixel_Data * data = &in;
    Pixel_Data tmp;
    Pixel_Data_Info info(in.size(), in.pixel());

    if (in.info() != info)
    {
        //DJV_DEBUG_PRINT("copy");
        copy(in, tmp);
        data = &tmp;
    }

    const int w = info.size.x, h = info.size.y;
    const double area = w * h;
    const int channels = Pixel::channels(info.pixel);

    switch (Pixel::type(info.pixel))
    {

#define _AVERAGE(TYPE) \
    \
    const Pixel::TYPE * p = reinterpret_cast< \
        const Pixel::TYPE *>(data->data()); \
    \
    double accum [Pixel::channels_max]; \
    \
    for (int c = 0; c < channels; ++c) accum[c] = 0.0; \
        for (int y = 0; y < h; ++y) \
            for (int x = 0; x < w; ++x, p += channels) \
                for (int c = 0; c < channels; ++c) \
                    accum[c] += p[c] / area;

        case Pixel::U8:
        {
            _AVERAGE(U8_T)

            for (int c = 0; c < channels; ++c)
            {
                out->set_u8(int(accum[c]), c);
            }
        }
        break;

        case Pixel::U16:
        {
            _AVERAGE(U16_T)

            for (int c = 0; c < channels; ++c)
            {
                out->set_u16(int(accum[c]), c);
            }
        }
        break;

        case Pixel::F16:
        {
            _AVERAGE(F16_T)

            for (int c = 0; c < channels; ++c)
            {
                out->set_f16(static_cast<Pixel::F16_T>(accum[c]), c);
            }
        }
        break;

        case Pixel::F32:
        {
            _AVERAGE(F32_T)

            for (int c = 0; c < channels; ++c)
            {
                out->set_f32(static_cast<Pixel::F32_T>(accum[c]), c);
            }
        }
        break;

        case Pixel::U10:
        {
            const Pixel::U10_S * p =
                reinterpret_cast <const Pixel::U10_S * > (data->data());
            
            double accum [3] = { 0.0, 0.0, 0.0 };

            for (int y = 0; y < h; ++y)
            {
                for (int x = 0; x < w; ++x, ++p)
                {
                    accum[0] += p->r / area;
                    accum[1] += p->g / area;
                    accum[2] += p->b / area;
                }
            }
            
            for (int c = 0; c < 3; ++c)
            {
                out->set_u10(static_cast<int>(accum[c]), c);
            }
        }
        break;

        default:
            break;
    }

    //DJV_DEBUG_PRINT("out = " << *out);
}

namespace
{

int shift_10(Gl_Image::HISTOGRAM in)
{
    switch (in)
    {
        case Gl_Image::HISTOGRAM_256:
            return 2;

        case Gl_Image::HISTOGRAM_1024:
            return 0;

        default:
            break;
    }

    return 0;
}

int shift_16(Gl_Image::HISTOGRAM in)
{
    switch (in)
    {
        case Gl_Image::HISTOGRAM_256:
            return 8;

        case Gl_Image::HISTOGRAM_1024:
            return 6;

        default:
            break;
    }

    return 0;
}

} // namespace

void Gl_Image::histogram(
    const Pixel_Data & in,
    Pixel_Data *       out,
    HISTOGRAM          histogram,
    Color *            min,
    Color *            max) throw (Error)
{
    DJV_ASSERT(out);

    //DJV_DEBUG("histogram");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("histogram = " << histogram_size(histogram));

    const int size = histogram_size(histogram);
    out->set(Pixel_Data_Info(V2i(size, 1), Pixel::RGB_U16));
    out->zero();

    //DJV_DEBUG_PRINT("out = " << *out);

    const Pixel_Data * data = &in;
    Pixel_Data tmp;
    Pixel_Data_Info info(in.size(), in.pixel());

    if (in.info() != info)
    {
        copy(in, tmp);
        data = &tmp;
    }

    *min = Color(info.pixel);
    *max = Color(info.pixel);

    const int w = info.size.x, h = info.size.y;
    const int channels = Pixel::channels(info.pixel);

    switch (info.pixel)
    {
        case Pixel::RGB_U10:
        {
            const Pixel::U10_S * in_p =
                reinterpret_cast<const Pixel::U10_S *>(data->data());
            const Pixel::U10_S * const end = in_p + w * h;
            Pixel::U16_T * const out_p =
                reinterpret_cast<Pixel::U16_T *>(out->data());
            Pixel::U10_S * const min_p =
                reinterpret_cast<Pixel::U10_S *>(min->data());
            Pixel::U10_S * const max_p =
                reinterpret_cast<Pixel::U10_S *>(max->data());

            if (w && h)
            {
                min_p->r = max_p->r = in_p->r;
                min_p->g = max_p->g = in_p->g;
                min_p->b = max_p->b = in_p->b;
            }

            const int shift = shift_10(histogram);

            for (; in_p < end; ++in_p)
            {
                out_p[(in_p->r >> shift) * 3 + 0]++;
                out_p[(in_p->g >> shift) * 3 + 1]++;
                out_p[(in_p->b >> shift) * 3 + 2]++;

                min_p->r = Math::min(in_p->r, min_p->r);
                min_p->g = Math::min(in_p->g, min_p->g);
                min_p->b = Math::min(in_p->b, min_p->b);
                max_p->r = Math::max(in_p->r, max_p->r);
                max_p->g = Math::max(in_p->g, max_p->g);
                max_p->b = Math::max(in_p->b, max_p->b);
            }

        }
        break;

#define _HISTOGRAM(TYPE) \
    \
    const Pixel::TYPE##_T * in_p = \
        reinterpret_cast<const Pixel::TYPE##_T *>(data->data()); \
    \
    const Pixel::TYPE##_T * const end = in_p + w * h * channels; \
    \
    Pixel::U16_T * const out_p = \
        reinterpret_cast<Pixel::U16_T *>(out->data()); \
    \
    Pixel::TYPE##_T * const min_p = \
        reinterpret_cast<Pixel::TYPE##_T *>(min->data()); \
    \
    Pixel::TYPE##_T * const max_p = \
        reinterpret_cast<Pixel::TYPE##_T *>(max->data()); \
    \
    if (w && h) \
        for (int c = 0; c < channels; ++c) \
            min_p[c] = max_p[c] = in_p[c]; \
    \
    const int shift = shift_16(histogram); \
    \
    for (; in_p < end; in_p += channels) \
        switch (channels) \
        { \
            case 4: \
                min_p[3] = Math::min(in_p[3], min_p[3]); \
                max_p[3] = Math::max(in_p[3], max_p[3]); \
            case 3: \
                out_p[(PIXEL_##TYPE##_TO_U16(in_p[2]) >> shift) * 3 + 2]++; \
                min_p[2] = Math::min(in_p[2], min_p[2]); \
                max_p[2] = Math::max(in_p[2], max_p[2]); \
            case 2: \
                out_p[(PIXEL_##TYPE##_TO_U16(in_p[1]) >> shift) * 3 + 1]++; \
                min_p[1] = Math::min(in_p[1], min_p[1]); \
                max_p[1] = Math::max(in_p[1], max_p[1]); \
            case 0: \
                out_p[(PIXEL_##TYPE##_TO_U16(in_p[0]) >> shift) * 3 + 0]++; \
                min_p[0] = Math::min(in_p[0], min_p[0]); \
                max_p[0] = Math::max(in_p[0], max_p[0]); \
                break; \
        }

        case Pixel::L_U8:
        case Pixel::LA_U8:
        case Pixel::RGB_U8:
        case Pixel::RGBA_U8:
        {
            _HISTOGRAM(U8);
        }
        break;

        case Pixel::L_U16:
        case Pixel::LA_U16:
        case Pixel::RGB_U16:
        case Pixel::RGBA_U16:
        {
            _HISTOGRAM(U16);
        }
        break;

        case Pixel::L_F16:
        case Pixel::LA_F16:
        case Pixel::RGB_F16:
        case Pixel::RGBA_F16:
        {
            _HISTOGRAM(F16);
        }
        break;

        case Pixel::L_F32:
        case Pixel::LA_F32:
        case Pixel::RGB_F32:
        case Pixel::RGBA_F32:
        {
            _HISTOGRAM(F32);
        }
        break;

        default:
            break;
    }

    /*#if defined(DJV_DEBUG)
      Pixel::U16_T * const out_p =
        reinterpret_cast<Pixel::U16_T *>(out->data());
      for (int i = 0; i < size; ++i)
        //DJV_DEBUG_PRINT(i << " = " << out_p[i * 3]);
    #endif*/
}

int Gl_Image::histogram_size(HISTOGRAM in)
{
    static const int data [] =
    {
        256,
        1024
    };

    return data[in];
}

const List<String> & Gl_Image::label_histogram()
{
    static const List<String> data = List<String>() <<
        "256" <<
        "1024";

    return data;
}

//------------------------------------------------------------------------------

bool operator == (const Gl_Image_Xform & a, const Gl_Image_Xform & b)
{
    return
        a.mirror == b.mirror &&
        a.position == b.position &&
        a.scale == b.scale &&
        a.rotate == b.rotate;
}

bool operator != (const Gl_Image_Xform & a, const Gl_Image_Xform & b)
{
    return ! (a == b);
}

bool operator == (const Gl_Image_Color & a, const Gl_Image_Color & b)
{
    return
        a.brightness == b.brightness &&
        a.contrast == b.contrast &&
        a.saturation == b.saturation;
}

bool operator != (const Gl_Image_Color & a, const Gl_Image_Color & b)
{
    return ! (a == b);
}

bool operator == (const Gl_Image_Levels & a, const Gl_Image_Levels & b)
{
    return
        a.in_low == b.in_low &&
        a.in_high == b.in_high &&
        a.gamma == b.gamma &&
        a.out_low == b.out_low &&
        a.out_high == b.out_high;
}

bool operator != (const Gl_Image_Levels & a, const Gl_Image_Levels & b)
{
    return ! (a == b);
}

bool operator == (
    const Gl_Image_Display_Profile & a,
    const Gl_Image_Display_Profile & b)
{
    return
        a.lut == b.lut &&
        a.color == b.color &&
        a.levels == b.levels &&
        a.soft_clip == b.soft_clip;
}

bool operator != (
    const Gl_Image_Display_Profile & a,
    const Gl_Image_Display_Profile & b)
{
    return ! (a == b);
}

bool operator == (const Gl_Image_Filter & a, const Gl_Image_Filter & b)
{
    return
        a.min == b.min &&
        a.mag == b.mag;
}

bool operator != (const Gl_Image_Filter & a, const Gl_Image_Filter & b)
{
    return ! (a == b);
}

bool operator == (const Gl_Image_Options & a, const Gl_Image_Options & b)
{
    return
        a.xform           == b.xform           &&
        a.color_profile   == b.color_profile   &&
        a.display_profile == b.display_profile &&
        a.channel         == b.channel         &&
        a.filter          == b.filter          &&
        a.background      == b.background;
}

bool operator != (const Gl_Image_Options & a, const Gl_Image_Options & b)
{
    return ! (a == b);
}

String & operator >> (String & in, Gl_Image_Xform & out) throw (String)
{
    return in >>
        out.mirror >>
        out.position >>
        out.scale >>
        out.rotate;
}

String & operator << (String & out, const Gl_Image_Xform & in)
{
    return out <<
        in.mirror <<
        in.position <<
        in.scale <<
        in.rotate;
}

String & operator >> (String & in, Gl_Image_Color & out) throw (String)
{
    return in >>
        out.brightness >>
        out.contrast >>
        out.saturation;
}

String & operator << (String & out, const Gl_Image_Color & in)
{
    return out <<
        in.brightness <<
        in.contrast <<
        in.saturation;
}

String & operator >> (String & in, Gl_Image_Levels & out) throw (String)
{
    return in >>
        out.in_low >>
        out.in_high >>
        out.gamma >>
        out.out_low >>
        out.out_high;
}

String & operator << (String & out, const Gl_Image_Levels & in)
{
    return out <<
        in.in_low <<
        in.in_high <<
        in.gamma <<
        in.out_low <<
        in.out_high;
}

String & operator >> (String & in, Gl_Image_Filter & out) throw (String)
{
    return in >>
        out.min >>
        out.mag;
}

String & operator << (String & out, const Gl_Image_Filter & in)
{
    return out <<
        in.min <<
        in.mag;
}

_DJV_STRING_OPERATOR_LABEL(
    Gl_Image_Filter::FILTER,
    Gl_Image_Filter::label_filter())
_DJV_STRING_OPERATOR_LABEL(
    Gl_Image_Options::CHANNEL,
    Gl_Image_Options::label_channel())
_DJV_STRING_OPERATOR_LABEL(
    Gl_Image::HISTOGRAM,
    Gl_Image::label_histogram())

Debug & operator << (Debug & debug, const Gl_Image_Xform & in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, const Gl_Image_Color & in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, const Gl_Image_Levels & in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, const Gl_Image_Filter & in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, Gl_Image_Filter::FILTER in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, Gl_Image_Options::CHANNEL in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, Gl_Image::HISTOGRAM in)
{
    return debug << String_Util::label(in);
}

} // djv

