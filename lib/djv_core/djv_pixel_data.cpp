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

//! \file djv_pixel_data.cpp

#include <djv_pixel_data.h>

#include <djv_box.h>
#include <djv_color.h>
#include <djv_file_io.h>

namespace djv
{

//------------------------------------------------------------------------------
// Pixel_Data_Info
//------------------------------------------------------------------------------

void Pixel_Data_Info::init()
{
    layer_name = "Default";
    proxy      = PROXY_NONE;
    pixel      = Pixel::PIXEL(0);
    bgr        = false;
    align      = 1;
    endian     = Memory::endian();
}

Pixel_Data_Info::Pixel_Data_Info()
{
    init();
}

Pixel_Data_Info::Pixel_Data_Info(
    const V2i &  size,
    Pixel::PIXEL pixel)
{
    init();

    this->size  = size;
    this->pixel = pixel;
}

Pixel_Data_Info::Pixel_Data_Info(
    const String & file_name,
    const V2i &    size,
    Pixel::PIXEL   pixel)
{
    init();

    this->file_name = file_name;
    this->size      = size;
    this->pixel     = pixel;
}

const List<String> & Pixel_Data_Info::label_proxy()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "1/2" <<
        "1/4" <<
        "1/8";

    DJV_ASSERT(data.size() == Pixel_Data_Info::_PROXY_SIZE);

    return data;
}

//------------------------------------------------------------------------------
// Pixel_Data
//------------------------------------------------------------------------------

void Pixel_Data::init()
{
    _channels       = 0;
    _bytes_pixel    = 0;
    _bytes_scanline = 0;
    _bytes_data     = 0;
    _p              = 0;
    _io             = 0;
}

Pixel_Data::Pixel_Data()
{
    //DJV_DEBUG("Pixel_Data::Pixel_Data");

    init();
}

Pixel_Data::Pixel_Data(const Pixel_Data & in)
{
    //DJV_DEBUG("Pixel_Data::Pixel_Data");

    init();

    set(in);
}

Pixel_Data::Pixel_Data(
    const Pixel_Data_Info & in,
    const uint8_t *         p,
    File_Io *               io)
{
    //DJV_DEBUG("Pixel_Data::Pixel_Data");

    init();

    set(in, p, io);
}

Pixel_Data::~Pixel_Data()
{
    delete _io;
}

Pixel_Data & Pixel_Data::operator = (const Pixel_Data & in)
{
    if (&in != this)
    {
        set(in);
    }

    return *this;
}

void Pixel_Data::set(const Pixel_Data & in)
{
    set(in._info);
    Memory::copy(in.data(), data(), _bytes_data);
}

void Pixel_Data::set(
    const Pixel_Data_Info & in,
    const uint8_t *         p,
    File_Io *               io)
{
    //DJV_DEBUG("Pixel_Data::Pixel_Data");
    //DJV_DEBUG_PRINT("in = " << in);

    delete _io;

    _info = in;

    _channels = Pixel::channels(_info.pixel);
    _bytes_pixel = Pixel::bytes(_info.pixel);
    _bytes_scanline = bytes_scanline(_info);
    _bytes_data = bytes_data(_info);

    //DJV_DEBUG_PRINT("channels = " << _channels);
    //DJV_DEBUG_PRINT("bytes pixel = " << _bytes_pixel);
    //DJV_DEBUG_PRINT("bytes scanline = " << _bytes_scanline);
    //DJV_DEBUG_PRINT("bytes data = " << _bytes_data);

    if (p)
    {
        _data.size(0);
        
        _p = p;
    }
    else
    {
        _data.size(_bytes_data);
        
        _p = _data();
    }
    
    _io = io;
}

void Pixel_Data::zero()
{
    //DJV_DEBUG("Pixel_Data::zero");

    _data.zero();
}

size_t Pixel_Data::bytes_scanline(const Pixel_Data_Info & in)
{
    return (in.size.x * Pixel::bytes(in.pixel) * in.align) / in.align;
}

size_t Pixel_Data::bytes_data(const Pixel_Data_Info & in)
{
    return in.size.y * bytes_scanline(in);
}

void Pixel_Data::proxy_scale(
    const Pixel_Data &     in,
    Pixel_Data *           out,
    Pixel_Data_Info::PROXY proxy)
{
    DJV_ASSERT(out);

    //DJV_DEBUG("Pixel_Data::proxy_scale");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("out = " << *out);
    //DJV_DEBUG_PRINT("proxy = " << proxy);

    const int  w           = out->w();
    const int  h           = out->h();
    const int  proxy_scale = Pixel_Data::proxy_scale(proxy);
    const bool bgr         = in.info().bgr != out->info().bgr;
    const bool endian      = in.info().endian != Memory::endian();

    const bool fast = in.pixel() == out->pixel() && ! bgr;

    Memory_Buffer<uint8_t> tmp;

    if (! fast && endian)
    {
        tmp.size(w * proxy_scale * Pixel::bytes(in.pixel()));
    }

    for (int y = 0; y < h; ++y)
    {
        const uint8_t * in_p = in.data(0, y * proxy_scale);
        uint8_t * out_p = out->data(0, y);

        if (fast)
        {
            const size_t bytes_pixel = in.bytes_pixel();
            const size_t in_stride   = bytes_pixel * proxy_scale;
            const size_t out_stride  = bytes_pixel;

            for (
                int x = 0;
                x < w;
                ++x, in_p += in_stride, out_p += out_stride)
            {
                switch (bytes_pixel)
                {
                    case 16: out_p[15] = in_p[15];
                    case 15: out_p[14] = in_p[14];
                    case 14: out_p[13] = in_p[13];
                    case 13: out_p[12] = in_p[12];
                    case 12: out_p[11] = in_p[11];
                    case 11: out_p[10] = in_p[10];
                    case 10: out_p[9]  = in_p[9];
                    case 9:  out_p[8]  = in_p[8];
                    case 8:  out_p[7]  = in_p[7];
                    case 7:  out_p[6]  = in_p[6];
                    case 6:  out_p[5]  = in_p[5];
                    case 5:  out_p[4]  = in_p[4];
                    case 4:  out_p[3]  = in_p[3];
                    case 3:  out_p[2]  = in_p[2];
                    case 2:  out_p[1]  = in_p[1];
                    case 1:  out_p[0]  = in_p[0];
                }
            }
        }
        else
        {
            if (endian)
            {
                Memory::endian(
                    in_p,
                    tmp(),
                    w * proxy_scale,
                    Pixel::bytes(in.pixel()));
                
                in_p = tmp();
            }

            Pixel::convert(
                in_p,
                in.pixel(),
                out_p,
                out->pixel(),
                w,
                proxy_scale,
                bgr);
        }
    }
}

int Pixel_Data::proxy_scale(Pixel_Data_Info::PROXY proxy)
{
    return proxy ? Math::pow(2, static_cast<int>(proxy)) : 1;
}

V2i Pixel_Data::proxy_scale(const V2i & in, Pixel_Data_Info::PROXY proxy)
{
    const int scale = proxy_scale(proxy);

    return V2i(
        Math::ceil(in.x / static_cast<double>(scale)),
        Math::ceil(in.y / static_cast<double>(scale)));
}

Box2i Pixel_Data::proxy_scale(const Box2i & in, Pixel_Data_Info::PROXY proxy)
{
    const int scale = proxy_scale(proxy);

    return Box2i(
        Math::ceil(in.x / static_cast<double>(scale)),
        Math::ceil(in.y / static_cast<double>(scale)),
        Math::ceil(in.size.x / static_cast<double>(scale)),
        Math::ceil(in.size.y / static_cast<double>(scale)));
}

void Pixel_Data::planar_interleave(
    const Pixel_Data & in,
    Pixel_Data * out,
    Pixel_Data_Info::PROXY proxy)
{
    DJV_ASSERT(out);
    DJV_ASSERT(in.pixel() == out->pixel());
    DJV_ASSERT(in.pixel() != Pixel::RGB_U10);

    //DJV_DEBUG("Pixel_Data::planar_interleave");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("out = " << *out);
    //DJV_DEBUG_PRINT("proxy = " << proxy);

    const int w = out->w(), h = out->h();
    const int proxy_scale = Pixel_Data::proxy_scale(proxy);
    const int channels = out->channels();
    const size_t bytes_pixel = out->bytes_pixel();
    const int channel_bytes = Pixel::channel_bytes(out->pixel());

    for (int c = 0; c < channels; ++c)
    {
        for (int y = 0; y < h; ++y)
        {
            const uint8_t * in_p = in.data() + (c * in.h() + y * proxy_scale) *
                in.w() * channel_bytes;
            
            uint8_t * out_p = out->data(0, y) + c * channel_bytes;

            for (
                int x = 0;
                x < w;
                ++x, in_p += channel_bytes * proxy_scale, out_p += bytes_pixel)
            {
                switch (channel_bytes)
                {
                    case 4:
                        out_p[3] = in_p[3];

                    case 3:
                        out_p[2] = in_p[2];

                    case 2:
                        out_p[1] = in_p[1];

                    case 1:
                        out_p[0] = in_p[0];
                }
            }
        }
    }
}

void Pixel_Data::planar_deinterleave(const Pixel_Data & in, Pixel_Data * out)
{
    DJV_ASSERT(out);
    DJV_ASSERT(in.pixel() == out->pixel());
    DJV_ASSERT(in.pixel() != Pixel::RGB_U10);

    //DJV_DEBUG("Pixel_Data::planar_deinterleave");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("out = " << *out);

    const int w = out->w(), h = out->h();
    const int channels = out->channels();
    const size_t bytes_pixel = out->bytes_pixel();
    const int channel_bytes = Pixel::channel_bytes(out->pixel());

    for (int c = 0; c < channels; ++c)
    {
        for (int y = 0; y < h; ++y)
        {
            const uint8_t * in_p = in.data(0, y) + c * channel_bytes;
            uint8_t * out_p = out->data() + (c * h + y) * w * channel_bytes;

            for (
                int x = 0;
                x < w;
                ++x, in_p += bytes_pixel, out_p += channel_bytes)
            {
                switch (channel_bytes)
                {
                    case 4:
                        out_p[3] = in_p[3];

                    case 3:
                        out_p[2] = in_p[2];

                    case 2:
                        out_p[1] = in_p[1];

                    case 1:
                        out_p[0] = in_p[0];
                }
            }
        }
    }
}

void Pixel_Data::gradient(Pixel_Data * out)
{
    DJV_ASSERT(out);

    //DJV_DEBUG("gradient");

    const Pixel_Data_Info info(out->size(), Pixel::L_F32);
    
    out->set(info);
    
    //DJV_DEBUG_PRINT("out = " << *out);

    for (int y = 0; y < info.size.y; ++y)
    {
        Pixel::F32_T * p = reinterpret_cast<Pixel::F32_T *>(out->data(0, y));

        for (int x = 0; x < info.size.x; ++x, ++p)
        {
            *p = static_cast<Pixel::F32_T>(
                x / static_cast<double>(info.size.x - 1));
        }
    }
}

//------------------------------------------------------------------------------

bool operator == (const Pixel_Data_Info & a, const Pixel_Data_Info & b)
{
    return
        a.size == b.size &&
        a.proxy == b.proxy &&
        a.pixel == b.pixel &&
        a.bgr == b.bgr &&
        a.mirror == b.mirror &&
        a.align == b.align &&
        a.endian == b.endian;
}

bool operator != (const Pixel_Data_Info & a, const Pixel_Data_Info & b)
{
    return ! (a == b);
}

bool operator == (const Pixel_Data & a, const Pixel_Data & b)
{
    return
        a.info() == b.info() &&
        a.bytes_data() == b.bytes_data() &&
        Memory::compare(a.data(), b.data(), a.bytes_data());
}

bool operator != (const Pixel_Data & a, const Pixel_Data & b)
{
    return ! (a == b);
}

_DJV_STRING_OPERATOR_LABEL(
    Pixel_Data_Info::PROXY,
    Pixel_Data_Info::label_proxy())

Debug & operator << (Debug & debug, Pixel_Data_Info::PROXY in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, const Pixel_Data_Info & in)
{
    return debug <<
        //"file name: " << in.file_name << ", " <<
        //"layer name: " << in.layer_name << ", " <<
        "size: " << in.size << ", " <<
        "proxy: " << in.proxy << ", " <<
        "pixel: " << in.pixel << ", " <<
        "bgr: " << in.bgr << ", " <<
        "mirror: " << in.mirror << ", " <<
        "align: " << in.align << ", " <<
        "endian: " << in.endian;
}

Debug & operator << (Debug & debug, const Pixel_Data & in)
{
    return debug << in.info();
}

} // djv

