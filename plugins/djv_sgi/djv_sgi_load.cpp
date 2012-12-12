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

//! \file djv_sgi_load.cpp

#include <djv_sgi_load.h>

namespace djv_sgi
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
    return djv_sgi::name;
}

List<String> Load::extensions() const
{
    return djv_sgi::extensions;
}

void Load::open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::open");
    //DJV_DEBUG_PRINT("in = " << in);

    _file = in;
    
    File_Io io;
    io.endian(Memory::endian() != Memory::MSB);

    _open(_file.get(_file.seq().start()), info, io);

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

    info.file_name = in;
    info_load(io, info, &_compression);

    // Read the scanline tables.

    if (_compression)
    {
        const int table_size = info.size.y * Pixel::channels(info.pixel);
        //DJV_DEBUG_PRINT("rle table size = " << table_size);

        _rle_offset.size(table_size);
        _rle_size.size(table_size);

        io.get_u32(_rle_offset(), table_size);
        io.get_u32(_rle_size(),   table_size);
    }
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
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

    const size_t position = io->position();
    const size_t size     = io->size() - position;
    const int channels    = Pixel::channels(info.pixel);
    const int bytes       = Pixel::channel_bytes(info.pixel);

    if (! _compression)
    {
        if (1 == bytes)
        {
            const uint8_t * p = io->mmap_p();
            
            io->seek(Pixel_Data::bytes_data(info));
            
            _tmp.set(info, p, io.release());
        }
        else
        {
            _tmp.set(info);
            
            io->get(_tmp.data(), size / bytes, bytes);
        }
    }
    else
    {
        _tmp.set(info);

        Memory_Buffer<uint8_t> tmp(size);
        
        io->get(tmp(), size / bytes, bytes);

        const uint8_t * in_p  = tmp();
        const uint8_t * end   = in_p + size;
        uint8_t *       out_p = _tmp.data();

        for (int c = 0; c < channels; ++c)
        {
            //DJV_DEBUG_PRINT("channel = " << c);

            for (int y = 0; y < info.size.y; ++y, out_p += info.size.x * bytes)
            {
                //DJV_DEBUG_PRINT("y = " << y);

                if (! rle_load(
                    in_p + _rle_offset()[y + info.size.y * c] - position,
                    end,
                    out_p,
                    info.size.x,
                    bytes,
                    io->endian()))
                {
                    throw_error_read(name(), file_name);
                }
            }
        }
    }

    // Interleave the image channels.

    info.size = Pixel_Data::proxy_scale(info.size, frame.proxy);
    info.proxy = frame.proxy;
    image.set(info);

    Pixel_Data::planar_interleave(_tmp, &image, frame.proxy);

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_sgi
