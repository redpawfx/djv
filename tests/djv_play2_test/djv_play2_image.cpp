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

//! \file djv_play2_image.cpp

#include <djv_play2_image.h>

#include <djv_gl_image.h>

namespace djv_play2_test
{

Image::Image() :
    _id(0)
{}

Image::~Image()
{
    del();
}

const Pixel_Data_Info & Image::info() const
{
    return _info;
}

void Image::del()
{
    if (_id)
    {
        glDeleteTextures(1, &_id);
        _id = 0;
    }
}

Image_Ppm::Image_Ppm() :
    _init(false),
    _load(false)
{}

void Image_Ppm::load(const String & in) throw (Error)
{
    //DJV_DEBUG("Image_Ppm::load");
    //DJV_DEBUG_PRINT("in = " << in);

    _io.open(in, File_Io::READ);
    _io.read_ahead();

    char magic [] = { 0, 0, 0 };
    _io.get(magic, 2);

    //DJV_DEBUG_PRINT("magic = " << magic);

    if (magic[0] != 'P')
    {
        throw Error("ppm magic");
    }

    if (magic[1] != '6')
    {
        throw Error("ppm type");
    }

    char tmp [cstring_size] = "";

    V2i size;
    File_Io::word(_io, tmp, cstring_size);
    size.x = String_Util::string_to_int<int>(tmp, cstring_size);
    File_Io::word(_io, tmp, cstring_size);
    size.y = String_Util::string_to_int<int>(tmp, cstring_size);

    int max_value = 0;
    File_Io::word(_io, tmp, cstring_size);
    max_value = String_Util::string_to_int<int>(tmp, cstring_size);

    if (max_value >= 256)
    {
        throw Error("ppm max value");
    }

    Pixel_Data_Info info(size, Pixel::RGB_U8);
    info.mirror.y = true;
    info.endian = Memory::MSB;

    //DJV_DEBUG_PRINT("info = " << info.size);

    if (info != _info)
    {
        _init = true;

        _info = info;
    }

    _load = true;
}

void Image_Ppm::bind() const throw (Error)
{
    Image_Ppm * that = const_cast<Image_Ppm *>(this);

    //DJV_DEBUG("Image_Ppm::bind");
    //DJV_DEBUG_PRINT("info = " << _info.size);

    const GLenum target = GL_TEXTURE_2D;
    const GLenum format = Gl_Util::format(_info.pixel, _info.bgr);
    const GLenum type = Gl_Util::type(_info.pixel);

    if (_init)
    {
        //DJV_DEBUG_PRINT("init");

        that->del();

        DJV_DEBUG_GL(glGenTextures(1, &that->_id));

        if (! _id)
        {
            throw Error("texture");
        }

        DJV_DEBUG_GL(glEnable(target));
        DJV_DEBUG_GL(glBindTexture(target, _id));

        DJV_DEBUG_GL(
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        DJV_DEBUG_GL(
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        DJV_DEBUG_GL(
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        DJV_DEBUG_GL(
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

        DJV_DEBUG_GL(
            glTexImage2D(
                target,
                0,
                GL_RGBA,
                _info.size.x,
                _info.size.y,
                0,
                format,
                type,
                0));

        that->_init = false;
    }

    DJV_DEBUG_GL(glEnable(target));
    DJV_DEBUG_GL(glBindTexture(target, _id));

    if (_load)
    {
        //DJV_DEBUG_PRINT("load");

        Gl_Image::state_unpack(_info);

        const uint8_t * p = _io.mmap_p();

        DJV_DEBUG_GL(
            glTexSubImage2D(
                target,
                0,
                0,
                0,
                _info.size.x,
                _info.size.y,
                format,
                type,
                p));

        that->_load = false;
    }
}

} // djv_play2_test

