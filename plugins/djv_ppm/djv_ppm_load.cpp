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

//! \file djv_ppm_load.cpp

#include <djv_ppm_load.h>

namespace djv_ppm
{

//------------------------------------------------------------------------------
// djv_image_load
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_load()
{
    return new Load;
}

}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Plugin * Load::copy() const
{
    return new Load;
}

String Load::name() const
{
    return djv_ppm::name;
}

List<String> Load::extensions() const
{
    return djv_ppm::extensions;
}

void Load::open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::open");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("type = " << in.type());

    _file = in;
    
    File_Io io;
    io.endian(Memory::endian() != Memory::MSB);
    
    _open(in.get(in.seq().start()), info, io);

    if (File::SEQ == _file.type())
    {
        info.seq.list = _file.seq().list;
    }
}

void Load::_open(const String & in, Image_Io_Info & info, File_Io & io)
    throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    // Open the file.

    io.open(in, File_Io::READ);

    char magic [] = { 0, 0, 0 };
    io.get(magic, 2);

    //DJV_DEBUG_PRINT("magic = " << magic);

    if (magic[0] != 'P')
    {
        throw_error_unrecognized(name(), in);
    }

    switch (magic[1])
    {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
            break;

        default:
            throw_error_unsupported(name(), in);
    }

    const int ppm_type = magic[1] - '0';

    //DJV_DEBUG_PRINT("ppm type = " << ppm_type);

    // Read the header.

    char tmp [cstring_size] = "";

    int width  = 0;
    int height = 0;
    
    File_Io::word(io, tmp, cstring_size);
    
    width = String_Util::string_to_int<int>(tmp, cstring_size);
    
    File_Io::word(io, tmp, cstring_size);
    
    height = String_Util::string_to_int<int>(tmp, cstring_size);

    int max_value = 0;

    if (ppm_type != 1 && ppm_type != 4)
    {
        File_Io::word(io, tmp, cstring_size);
        
        max_value = String_Util::string_to_int<int>(tmp, cstring_size);
    }

    //DJV_DEBUG_PRINT("max value = " << max_value);

    // Information.

    info.file_name = in;

    info.size = V2i(width, height);

    info.mirror.y = true;

    if (1 == ppm_type || 4 == ppm_type)
    {
        _bit_depth = 1;
    }
    else
    {
        _bit_depth = max_value < 256 ? 8 : 16;
    }

    //DJV_DEBUG_PRINT("bit depth = " << _bit_depth);

    int channels = 0;

    switch (ppm_type)
    {
        case 1:
        case 2:
        case 4:
        case 5:
            channels = 1;
            break;

        case 3:
        case 6:
            channels = 3;
            break;
    }

    if (! Pixel::pixel_(
            channels,
            _bit_depth != 1 ? _bit_depth : 8,
            false,
            &info.pixel))
    {
        throw_error_unsupported(name(), in);
    }

    _data =
        (1 == ppm_type || 2 == ppm_type || 3 == ppm_type) ?
        DATA_ASCII :
        DATA_BINARY;

    if (DATA_BINARY == _data)
    {
        info.endian = Memory::MSB;
    }
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Load::get");
    //DJV_DEBUG_PRINT("frame = " << frame);

    image.color_profile = Color_Profile();
    image.tag = Image_Tag();

    // Open the file.

    const String file_name =
        _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info;
    
    std::auto_ptr<File_Io> io(new File_Io);
    io->endian(Memory::endian() != Memory::MSB);
    
    _open(file_name, info, *io);

    // Read the file.

    io->read_ahead();

    Pixel_Data * data = frame.proxy ? &_tmp : &image;

    if (DATA_BINARY == _data && _bit_depth != 1)
    {
        const uint8_t * p = io->mmap_p();
        
        io->seek(Pixel_Data::bytes_data(info));
        
        data->set(info, p, io.release());
    }
    else
    {
        data->set(info);
        
        const int channels = Pixel::channels(info.pixel);

        if (DATA_BINARY == _data && 1 == _bit_depth)
        {
            const size_t bytes_scanline = djv_ppm::bytes_scanline(
                info.size.x, channels, _bit_depth, _data);
            
            Memory_Buffer<uint8_t> scanline(bytes_scanline);

            //DJV_DEBUG_PRINT("scanline = " <<
            //    static_cast<int>(bytes_scanline));

            for (int y = 0; y < info.size.y; ++y)
            {
                io->get(scanline(), bytes_scanline);
                
                const uint8_t * in_p = scanline();
                
                uint8_t * out_p = data->data(0, y);

                for (int i = info.size.x - 1; i >= 0; --i)
                {
                    out_p[i] = (in_p[i / 8] >> (7 - (i % 8))) & 1 ? 0 : 255;
                }
            }
        }
        else
        {
            for (int y = 0; y < info.size.y; ++y)
            {
                ascii_load(
                    *io,
                    data->data(0, y),
                    info.size.x * channels,
                    _bit_depth);
            }
        }
    }

    // Proxy scale the image.
    
    if (frame.proxy)
    {
        info.size = Pixel_Data::proxy_scale(info.size, frame.proxy);
        info.proxy = frame.proxy;
        
        image.set(info);

        Pixel_Data::proxy_scale(_tmp, &image, frame.proxy);
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_ppm

