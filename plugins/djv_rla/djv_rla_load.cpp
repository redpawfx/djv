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

//! \file djv_rla_load.cpp

#include <djv_rla_load.h>

namespace djv_rla
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

Load::Load()
{
    _io.endian(Memory::endian() != Memory::MSB);
}

Plugin * Load::copy() const
{
    return new Load;
}

String Load::name() const
{
    return djv_rla::name;
}

List<String> Load::extensions() const
{
    return djv_rla::extensions;
}

void Load::open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::open");
    //DJV_DEBUG_PRINT("in = " << in);

    _file = in;
    _open(_file.get(_file.seq().start()), info);

    if (File::SEQ == _file.type())
    {
        info.seq.list = _file.seq().list;
    }
}

namespace
{

struct Header
{
    int16_t dimensions [4]; // Left, right, bottom, top.
    int16_t active [4];
    int16_t frame;
    int16_t color_channel_type;
    int16_t color_channels;
    int16_t matte_channels;
    int16_t aux_channels;
    int16_t version;
    char    gamma [16];
    char    chroma [3][24];
    char    whitepoint [24];
    int32_t job;
    char    file_name [128];
    char    description [128];
    char    progam [64];
    char    machine [32];
    char    user [32];
    char    date [20];
    char    aspect [24];
    char    aspect_ratio [8];
    char    color_format [32];
    int16_t field;
    char    render_time [12];
    char    filter [32];
    int16_t color_bit_depth;
    int16_t matte_channel_type;
    int16_t matte_bit_depth;
    int16_t aux_channel_type;
    int16_t aux_bit_depth;
    char    aux_format [32];
    char    pad [36];
    int32_t offset;
};

void endian(Header * in)
{
    Memory::endian(&in->dimensions, 4, 2);
    Memory::endian(&in->active, 4, 2);
    Memory::endian(&in->frame, 1, 2);
    Memory::endian(&in->color_channel_type, 1, 2);
    Memory::endian(&in->color_channels, 1, 2);
    Memory::endian(&in->matte_channels, 1, 2);
    Memory::endian(&in->aux_channels, 1, 2);
    Memory::endian(&in->version, 1, 2);
    Memory::endian(&in->job, 1, 4);
    Memory::endian(&in->field, 1, 2);
    Memory::endian(&in->color_bit_depth, 1, 2);
    Memory::endian(&in->matte_channel_type, 1, 2);
    Memory::endian(&in->matte_bit_depth, 1, 2);
    Memory::endian(&in->aux_channel_type, 1, 2);
    Memory::endian(&in->aux_bit_depth, 1, 2);
    Memory::endian(&in->offset, 1, 4);
}

void debug(const Header & in)
{
    //DJV_DEBUG("debug(Header)");
    //DJV_DEBUG_PRINT("dimensions = " << in.dimensions[0] << " " <<
    //    in.dimensions[1] << " " << in.dimensions[2] << " " << "
    //    in.dimensions[3]);
    //DJV_DEBUG_PRINT("active = " << in.active[0] << " " << in.active[1] <<
    //    " " << in.active[2] << " " << in.active[3]);
    //DJV_DEBUG_PRINT("frame = " << in.frame);
    //DJV_DEBUG_PRINT("color channel type = " << in.color_channel_type);
    //DJV_DEBUG_PRINT("color channels = " << in.color_channels);
    //DJV_DEBUG_PRINT("matte channels = " << in.matte_channels);
    //DJV_DEBUG_PRINT("aux channels = " << in.aux_channels);
    //DJV_DEBUG_PRINT("version = " << in.version);
    //DJV_DEBUG_PRINT("gamma = " << in.gamma);
    //DJV_DEBUG_PRINT("chroma = " << in.chroma[0] << " " << in.chroma[1] <<
    //    " " << in.chroma[2]);
    //DJV_DEBUG_PRINT("whitepoint = " << in.whitepoint);
    //DJV_DEBUG_PRINT("job = " << in.job);
    //DJV_DEBUG_PRINT("file name = " << in.file_name);
    //DJV_DEBUG_PRINT("description = " << in.description);
    //DJV_DEBUG_PRINT("progam = " << in.progam);
    //DJV_DEBUG_PRINT("machine = " << in.machine);
    //DJV_DEBUG_PRINT("user = " << in.user);
    //DJV_DEBUG_PRINT("date = " << in.date);
    //DJV_DEBUG_PRINT("aspect = " << in.aspect);
    //DJV_DEBUG_PRINT("aspect ratio = " << in.aspect_ratio);
    //DJV_DEBUG_PRINT("color format = " << in.color_format);
    //DJV_DEBUG_PRINT("field = " << in.field);
    //DJV_DEBUG_PRINT("render time = " << in.render_time);
    //DJV_DEBUG_PRINT("filter = " << in.filter);
    //DJV_DEBUG_PRINT("color bit depth = " << in.color_bit_depth);
    //DJV_DEBUG_PRINT("matte channel type = " << in.matte_channel_type);
    //DJV_DEBUG_PRINT("matte bit depth = " << in.matte_bit_depth);
    //DJV_DEBUG_PRINT("aux channel type = " << in.aux_channel_type);
    //DJV_DEBUG_PRINT("aux bit depth = " << in.aux_bit_depth);
    //DJV_DEBUG_PRINT("aux format = " << in.aux_format);
    //DJV_DEBUG_PRINT("offset = " << in.offset);
}

} // namespace

void Load::_open(const String & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    // Open the file.

    _io.open(in, File_Io::READ);

    // Read the header.

    Header header;

    //DJV_DEBUG_PRINT("header size = " << static_cast<int>(sizeof(Header)));

    _io.get(&header, sizeof(Header));

    if (_io.endian())
    {
        endian(&header);
    }

    debug(header);

    const int w = header.active[1] - header.active[0] + 1;
    const int h = header.active[3] - header.active[2] + 1;

    // Read the scanline table.

    _rle_offset.size(h);
    _io.get_32(_rle_offset(), h);

    // Get file information.

    const V2i size(w, h);
    
    Pixel::PIXEL pixel(Pixel::PIXEL(0));

    if (header.matte_channels > 1)
    {
        throw_error_unsupported(name(), in);
    }

    if (header.matte_channel_type != header.color_channel_type)
    {
        throw_error_unsupported(name(), in);
    }

    if (header.matte_bit_depth != header.color_bit_depth)
    {
        throw_error_unsupported(name(), in);
    }

    if (! Pixel::pixel_(
        header.color_channels + header.matte_channels,
        header.color_bit_depth,
        3 == header.color_channel_type,
        &pixel))
    {
        throw_error_unsupported(name(), in);
    }

    if (header.field)
    {
        throw_error_unsupported(name(), in);
    }

    info = Pixel_Data_Info(in, size, pixel);
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
    _open(file_name, info);

    if (frame.layer < 0 ||
        frame.layer >= static_cast<int>(info.layer_size()))
    {
        throw_error_read(name(), file_name);
    }

    Pixel_Data_Info _info = info[frame.layer];

    // Read the file.

    _io.read_ahead();

    Pixel_Data * p = frame.proxy ? &_tmp : &image;
    
    p->set(_info);

    const int w        = _info.size.x;
    const int h        = _info.size.y;
    const int channels = Pixel::channels(_info.pixel);
    const int bytes    = Pixel::channel_bytes(_info.pixel);

    //DJV_DEBUG_PRINT("channels = " << channels);
    //DJV_DEBUG_PRINT("bytes = " << bytes);

    uint8_t * data_p = p->data();

    for (int y = 0; y < h; ++y, data_p += w * channels * bytes)
    {
        _io.position(_rle_offset()[y]);

        for (int c = 0; c < channels; ++c)
        {
            if (Pixel::F32 == Pixel::type(_info.pixel))
            {
                float_load(_io, data_p + c * bytes, w, channels);
            }
            else
            {
                rle_load(_io, data_p + c * bytes, w, channels, bytes);
            }
        }
    }

    // Proxy scale the image.
    
    if (frame.proxy)
    {
        _info.size = Pixel_Data::proxy_scale(_info.size, frame.proxy);
        _info.proxy = frame.proxy;
        
        image.set(_info);

        Pixel_Data::proxy_scale(_tmp, &image, frame.proxy);
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_rla
