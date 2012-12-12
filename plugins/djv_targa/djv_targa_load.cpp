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

//! \file djv_targa_load.cpp

#include <djv_targa_load.h>

namespace djv_targa
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
    return djv_targa::name;
}

List<String> Load::extensions() const
{
    return djv_targa::extensions;
}

void Load::open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::open");
    //DJV_DEBUG_PRINT("in = " << in);

    _file = in;
    
    File_Io io;
    io.endian(Memory::endian() != Memory::LSB);
    
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

    io.open(in, File_Io::READ);

    info.file_name = in;
    info_load(io, info, &_compression);
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::load");
    //DJV_DEBUG_PRINT("frame = " << frame);

    image.color_profile = Color_Profile();
    image.tag = Image_Tag();

    // Open the file.

    const String file_name =
        _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());

    //DJV_DEBUG_PRINT("file name = " << file_name);

    Image_Io_Info info;
    
    std::auto_ptr<File_Io> io(new File_Io);
    io->endian(Memory::endian() != Memory::LSB);
    
    _open(file_name, info, *io);

    // Read the file.

    io->read_ahead();

    Pixel_Data * data = frame.proxy ? &_tmp : &image;

    if (! _compression)
    {
        const uint8_t * p = io->mmap_p();
        
        io->seek(Pixel_Data::bytes_data(info));
        
        data->set(info, p, io.release());
    }
    else
    {
        data->set(info);

        const uint8_t * p = io->mmap_p();
        
        const uint8_t * const end = io->mmap_end();
        
        const int channels = Pixel::channels(info.pixel);

        for (int y = 0; y < info.size.y; ++y)
        {
            //DJV_DEBUG_PRINT("y = " << y);

            p = rle_load(p, end, data->data(0, y), info.size.x, channels);

            if (! p)
            {
                throw_error_read(name(), file_name);
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

} // djv_targa
