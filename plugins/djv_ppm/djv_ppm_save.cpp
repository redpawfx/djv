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

//! \file djv_ppm_save.cpp

#include <djv_ppm_save.h>

#include <djv_gl_image.h>

#include <stdio.h>

namespace djv_ppm
{

//------------------------------------------------------------------------------
// djv_image_save
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_save()
{
    return new Save;
}

}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Type" <<
        "Data";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Save::Save() :
    _type     (TYPE_AUTO),
    _data     (DATA_BINARY),
    _bit_depth(0)
{
    _io.endian(Memory::endian() != Memory::MSB);
}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_type = _type;
    out->_data = _data;
    return out;
}

String Save::name() const
{
    return djv_ppm::name;
}

List<String> Save::extensions() const
{
    return djv_ppm::extensions;
}

bool Save::option(const String & in, String * data)
{
    try
    {
        if (String_Util::compare_no_case(in, options()[TYPE_OPTION]))
        {
            *data >> _type;
        }
        else if (String_Util::compare_no_case(in, options()[DATA_OPTION]))
        {
            *data >> _data;
        }
    }
    catch (String)
    {
        return false;
    }

    return true;
}

String Save::option(const String & in) const
{
    String out;

    if (String_Util::compare_no_case(in, options()[TYPE_OPTION]))
    {
        out << _type;
    }
    else if (String_Util::compare_no_case(in, options()[DATA_OPTION]))
    {
        out << _data;
    }

    return out;
}

List<String> Save::options() const
{
    return label_options();
}

void Save::open(const File & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::open");
    //DJV_DEBUG_PRINT("in = " << in);

    _file = in;

    if (info.seq.list.size() > 1)
    {
        _file.type(File::SEQ);
    }

    _info = Pixel_Data_Info();
    _info.size = info.size;
    _info.mirror.y = true;

    switch (_type)
    {
        case TYPE_AUTO:
        {
            Pixel::FORMAT format = Pixel::format(info.pixel);

            switch (format)
            {
                case Pixel::LA:
                    format = Pixel::L;
                    break;

                case Pixel::RGBA:
                    format = Pixel::RGB;
                    break;

                default:
                    break;
            }

            Pixel::TYPE type = Pixel::type(info.pixel);

            switch (type)
            {
                case Pixel::U10:
                case Pixel::F16:
                case Pixel::F32:
                    type = Pixel::U16;
                    break;

                default:
                    break;
            }

            _info.pixel = Pixel::pixel(format, type);
            _bit_depth = Pixel::bit_depth(_info.pixel);
        }
        break;

        case TYPE_U1:
            _info.pixel = Pixel::L_U8;
            _bit_depth = 1;
            break;

        default:
            break;
    }

    _info.endian = Memory::MSB;

    //DJV_DEBUG_PRINT("info = " << _info);

    _image.set(_info);
}

void Save::_open(const String & in) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    // Open.

    _io.open(in, File_Io::WRITE);

    // Header.

    int ppm_type = 0;

    if (1 == _bit_depth)
    {
        ppm_type = DATA_ASCII == _data ? 1 : 4;
    }
    else
    {
        ppm_type = DATA_ASCII == _data ? 2 : 5;

        if (3 == _image.channels())
        {
            ++ppm_type;
        }
    }

    char magic [] = "P \n";
    magic[1] = '0' + ppm_type;
    _io.set(magic, 3);

    char tmp [cstring_size] = "";
    
    int size = SNPRINTF(
        tmp,
        cstring_size, "%d %d\n",
        _image.w(),
        _image.h());
    
    _io.set(tmp, size);

    if (_bit_depth != 1)
    {
        //! \todo The symbol Pixel::u16_max is undefined on Mac OS 10.6?

        //const int max_value =
        //    (8 == _bit_depth) ? Pixel::u8_max : Pixel::u16_max;
        
        const int max_value = (8 == _bit_depth) ? Pixel::u8_max : 65535;
        
        size = SNPRINTF(tmp, cstring_size, "%d\n", max_value);
        
        _io.set(tmp, size);
    }
}

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("type = " << _type);
    //DJV_DEBUG_PRINT("data = " << _data);

    // Open the file.

    _open(_file.get(frame.frame));

    // Convert the image.

    const Pixel_Data * p = &in;

    if (in.info() != _info)
    {
        //DJV_DEBUG_PRINT("convert = " << _image);

        _image.zero();

        Gl_Image::copy(in, _image);

        p = &_image;
    }

    // Write the file.

    if (DATA_BINARY == _data && _bit_depth != 1)
    {
        _io.set(p->data(), p->bytes_data());
    }
    else
    {
        const int w = p->w(), h = p->h();
        
        const int channels = Pixel::channels(p->info().pixel);
        
        const size_t bytes_scanline = djv_ppm::bytes_scanline(
            w,
            channels,
            _bit_depth,
            _data);

        Memory_Buffer<uint8_t> scanline(bytes_scanline);

        //DJV_DEBUG_PRINT("scanline = " << static_cast<int>(bytes_scanline));

        for (int y = 0; y < h; ++y)
        {
            if (DATA_BINARY == _data && 1 == _bit_depth)
            {
                const uint8_t * in_p = p->data(0, y);
                uint8_t * out_p = scanline();

                for (int i = 0; i < w; ++i)
                {
                    const int tmp = in_p[i];
                    const int off = i % 8;
                    const int j = i / 8;

                    if (0 == off)
                    {
                        out_p[j] = 0;
                    }

                    out_p[j] |= ((! tmp) & 1) << (7 - off);
                }

                _io.set(scanline(), bytes_scanline);
            }
            else
            {
                const size_t size = ascii_save(
                    p->data(0, y),
                    scanline(),
                    w * channels,
                    _bit_depth);
                
                _io.set(scanline(), size);
            }
        }
    }

    _io.close();
}

} // djv
