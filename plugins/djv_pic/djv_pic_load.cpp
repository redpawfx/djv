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

//! \file djv_pic_load.cpp

#include <djv_pic_load.h>

namespace djv_pic
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

Load::Load() :
    _type(TYPE(0))
{
    _io.endian(Memory::endian() != Memory::MSB);

    _compression[0] = false;
    _compression[1] = false;
}

Plugin * Load::copy() const
{
    return new Load;
}

String Load::name() const
{
    return djv_pic::name;
}

List<String> Load::extensions() const
{
    return djv_pic::extensions;
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
    uint32_t magic;
    float    version;
    char     comment [80];
    char     id [4];
    uint16_t width;
    uint16_t height;
    float    ratio;
    uint16_t fields;
    uint8_t  pad [2];
};

enum CHANNEL
{
    CHANNEL_A = 0x10,
    CHANNEL_B = 0x20,
    CHANNEL_G = 0x40,
    CHANNEL_R = 0x80
};

struct Channel
{
    uint8_t chained;
    uint8_t size;
    uint8_t type;
    uint8_t channel;
};

void _channel(File_Io & io, Channel * out)
{
    io.get_u8(&out->chained);
    io.get_u8(&out->size);
    io.get_u8(&out->type);
    io.get_u8(&out->channel);
}

/*String debug_channel(int in)
{
  String out;
  if (in & CHANNEL_R) out += 'R';
  if (in & CHANNEL_G) out += 'G';
  if (in & CHANNEL_B) out += 'B';
  if (in & CHANNEL_A) out += 'A';
  return out;
}*/

} /// namespace

void Load::_open(const String & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    // Open the file.

    _io.open(in, File_Io::READ);

    // Read the header.

    Header header;
    Memory::zero(&header, sizeof(Header));

    _io.get_u32(&header.magic);

    if (header.magic != 0x5380F634)
    {
        throw_error_unrecognized(name(), in);
    }

    _io.get_f32(&header.version);
    _io.get(header.comment, sizeof(header.comment));
    _io.get(header.id, sizeof(header.id));
    _io.get_u16(&header.width);
    _io.get_u16(&header.height);
    _io.get_f32(&header.ratio);
    _io.get_u16(&header.fields);
    _io.seek(sizeof(header.pad));

    //DJV_DEBUG_PRINT("version = " << header.version);
    //DJV_DEBUG_PRINT("comment = " <<
    //    String(header.comment, sizeof(header.comment)));
    //DJV_DEBUG_PRINT("id = " << String(header.id, sizeof(header.id)));
    //DJV_DEBUG_PRINT("width = " << header.width);
    //DJV_DEBUG_PRINT("height = " << header.height);
    //DJV_DEBUG_PRINT("ratio = " << header.ratio);
    //DJV_DEBUG_PRINT("fields = " << header.fields);

    if (String(header.id, sizeof(header.id)) != "PICT")
    {
        throw_error_unsupported(name(), in);
    }

    // Information.

    info.file_name = in;

    info.size = V2i(header.width, header.height);

    info.mirror.y = true;

    Channel channel;
    Memory::zero(&channel, sizeof(Channel));
    _channel(_io, &channel);
    
    _compression[0] = 2 == channel.type;

    //DJV_DEBUG_PRINT("channel = " << debug_channel(channel.channel));

    int type = -1;

    if ((CHANNEL_R & channel.channel) &&
        (CHANNEL_G & channel.channel) &&
        (CHANNEL_B & channel.channel) &&
        (CHANNEL_A & channel.channel) &&
        8 == channel.size &&
        ! channel.chained)
    {
        type = TYPE_RGBA;
    }
    else if (
        (CHANNEL_R & channel.channel) &&
        (CHANNEL_G & channel.channel) &&
        (CHANNEL_B & channel.channel) &&
        8 == channel.size &&
        ! channel.chained)
    {
        type = TYPE_RGB;
    }
    else if (
        (CHANNEL_R & channel.channel) &&
        (CHANNEL_G & channel.channel) &&
        (CHANNEL_B & channel.channel) &&
        8 == channel.size &&
        channel.chained)
    {
        Memory::zero(&channel, sizeof(Channel));
        _channel(_io, &channel);
        
        _compression[1] = 2 == channel.type;

        //DJV_DEBUG_PRINT("channel = " << debug_channel(channel.channel));

        if (! (CHANNEL_R & channel.channel) &&
            ! (CHANNEL_G & channel.channel) &&
            ! (CHANNEL_B & channel.channel) &&
            (CHANNEL_A & channel.channel) &&
            8 == channel.size &&
            ! channel.chained)
        {
            type = TYPE_RGB_A;
        }
    }

    if (-1 == type)
    {
        throw_error_unsupported(name(), in);
    }

    _type = TYPE(type);

    switch (_type)
    {
        case TYPE_RGB:
            info.pixel = Pixel::RGB_U8;
            break;

        case TYPE_RGBA:
        case TYPE_RGB_A:
            info.pixel = Pixel::RGBA_U8;
            break;

        default:
            break;
    }

    // Read image tags.

    if (header.comment[0])
        info.tag[Image_Tag::label_tag()[Image_Tag::DESCRIPTION]] =
            String(header.comment, sizeof(header.comment));

    //DJV_DEBUG_PRINT("info = " << info);
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
    _open(file_name, info);
    image.tag = info.tag;

    // Read the file.

    _io.read_ahead();

    Pixel_Data * data = frame.proxy ? &_tmp : &image;
    data->set(info);

    const int channels = Pixel::channels(info.pixel);
    const int bytes = Pixel::channel_bytes(info.pixel);
    const bool endian = _io.endian();

    const uint8_t * p = _io.mmap_p(), * const end = _io.mmap_end();

    for (int y = 0; y < info.size.y; ++y)
    {
        //DJV_DEBUG_PRINT("y = " << y);

        switch (_type)
        {
            case TYPE_RGB:
            case TYPE_RGBA:

                if (_compression[0])
                {
                    p = rle_load(
                        p,
                        end,
                        data->data(0, y),
                        info.size.x,
                        channels,
                        channels * bytes,
                        endian);

                    if (! p)
                    {
                        throw_error_read(name(), file_name);
                    }
                }
                else
                {
                    const int size = info.size.x * channels * bytes;
                    Memory::copy(p, data->data(0, y), size);

                    p += size;
                }

                break;

            case TYPE_RGB_A:

                if (_compression[0])
                {
                    p = rle_load(
                        p,
                        end,
                        data->data(0, y),
                        info.size.x,
                        3,
                        channels * bytes,
                        endian);

                    if (! p)
                    {
                        throw_error_read(name(), file_name);
                    }
                }
                else
                {
                    const int size = info.size.x * 3 * bytes;
                    Memory::copy(p, data->data(0, y), size);

                    p += size;
                }

                if (_compression[1])
                {
                    p = rle_load(
                        p,
                        end,
                        data->data(0, y) + 3 * bytes,
                        info.size.x,
                        1,
                        channels * bytes,
                        endian);

                    if (! p)
                    {
                        throw_error_read(name(), file_name);
                    }
                }
                else
                {
                    const int size = info.size.x * 1 * bytes;
                    Memory::copy(p, data->data(0, y), size);

                    p += size;
                }

                break;

            default:
                break;
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

} // djv_pic
