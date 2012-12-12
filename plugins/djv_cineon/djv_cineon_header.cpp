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

//! \file djv_cineon_header.cpp

#include <djv_cineon_header.h>

namespace djv_cineon
{

//------------------------------------------------------------------------------
// Header
//------------------------------------------------------------------------------

const uint32_t Header::magic [] =
{
    0x802a5fd7,
    0xd75f2a80
};

const float Header::speed_min = 0.000001f; //! \todo Is this still necessary?

Header::Header()
{
    Memory::set<uint8_t>(0xff, &file, sizeof(File));
    zero(file.version, 8);
    zero(file.name, 100);
    zero(file.time, 24);

    Memory::set<uint8_t>(0xff, &image, sizeof(Image));

    for (uint i = 0; i < 8; ++i)
    {
        zero(&image.channel[i].low_data);
        zero(&image.channel[i].low_quantity);
        zero(&image.channel[i].high_data);
        zero(&image.channel[i].high_quantity);
    }

    Memory::set<uint8_t>(0xff, &source, sizeof(Source));
    zero(&source.offset[0]);
    zero(&source.offset[1]);
    zero(source.file, 100);
    zero(source.time, 24);
    zero(source.input_device, 64);
    zero(source.input_model, 32);
    zero(source.input_serial, 32);
    zero(&source.input_pitch[0]);
    zero(&source.input_pitch[1]);
    zero(&source.gamma);

    Memory::set<uint8_t>(0xff, &film, sizeof(Film));
    zero(film.format, 32);
    zero(&film.frame_rate);
    zero(film.frame_id, 32);
    zero(film.slate, 200);
}

void Header::load(
    File_Io & io,
    Image_Io_Info & info,
    bool & film_print) throw (Error)
{

    //DJV_DEBUG("Header::load");

    // Read.

    io.get(&file, sizeof(File));

    bool endian = false;

    if (magic[0] == file.magic)
        ;
    else if (magic[1] == file.magic)
    {
        endian = true;
    }
    else
    {
        Image_Io_Base::throw_error_unrecognized(name, io.file_name());
    }

    io.get(&image, sizeof(Image));
    io.get(&source, sizeof(Source));
    io.get(&film, sizeof(Film));

    if (endian)
    {
        //DJV_DEBUG_PRINT("endian");
        io.endian(true);
        this->endian();
        info.endian = Memory::endian_opposite(Memory::endian());
    }

    // Information.

    if (file.image_offset)
    {
        io.position(file.image_offset);
    }

    switch (image.orient)
    {
        case ORIENT_LEFT_RIGHT_TOP_BOTTOM:
            info.mirror.y = true;
            break;

        case ORIENT_LEFT_RIGHT_BOTTOM_TOP:
            break;

        case ORIENT_RIGHT_LEFT_TOP_BOTTOM:
            info.mirror.x = true;
            break;

        case ORIENT_RIGHT_LEFT_BOTTOM_TOP:
            info.mirror.x = true;
            info.mirror.y = true;
            break;

        case ORIENT_TOP_BOTTOM_LEFT_RIGHT:
        case ORIENT_TOP_BOTTOM_RIGHT_LEFT:
        case ORIENT_BOTTOM_TOP_LEFT_RIGHT:
        case ORIENT_BOTTOM_TOP_RIGHT_LEFT:
            break;
    }

    if (! image.channels)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    int i = 1;

    for (; i < image.channels; ++i)
    {
        if ((image.channel[i].size[0] != image.channel[0].size[0]) ||
            (image.channel[i].size[1] != image.channel[0].size[1]))
        {
            break;
        }

        if (image.channel[i].bit_depth != image.channel[0].bit_depth)
        {
            break;
        }
    }

    if (i < image.channels)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    int pixel = -1;

    switch (image.channels)
    {
        case 3:
            switch (image.channel[0].bit_depth)
            {
                case 10:
                    pixel = Pixel::RGB_U10;
                    break;
            }

            break;
    }

    if (-1 == pixel)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    info.pixel = Pixel::PIXEL(pixel);

    info.size = V2i(
        image.channel[0].size[0],
        image.channel[0].size[1]);

    if (is_valid(&image.line_padding) && image.line_padding)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    if (is_valid(&image.channel_padding) && image.channel_padding)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    film_print = DESCRIPTOR_R_FILM_PRINT == image.channel[0].descriptor[1];

    // File image tags.

    const List<String> & label = Image_Tag::label_tag();
    const List<String> & label_cineon = label_tag();

    if (is_valid(file.time, 24))
    {
        info.tag[label[Image_Tag::TIME]] = to_string(file.time, 24);
    }

    // Source image tags.

    if (is_valid(&source.offset[0]) && is_valid(&source.offset[1]))
        info.tag[label_cineon[TAG_SOURCE_OFFSET]] =
            String_Util::join(List<String>() <<
                String_Util::int_to_string(source.offset[0]) <<
                String_Util::int_to_string(source.offset[1]),
                " ");

    if (is_valid(source.file, 100))
    {
        info.tag[label_cineon[TAG_SOURCE_FILE]] = to_string(source.file, 100);
    }

    if (is_valid(source.time, 24))
    {
        info.tag[label_cineon[TAG_SOURCE_TIME]] = to_string(source.time, 24);
    }

    if (is_valid(source.input_device, 64))
        info.tag[label_cineon[TAG_SOURCE_INPUT_DEVICE]] =
            to_string(source.input_device, 64);

    if (is_valid(source.input_model, 32))
        info.tag[label_cineon[TAG_SOURCE_INPUT_MODEL]] =
            to_string(source.input_model, 32);

    if (is_valid(source.input_serial, 32))
        info.tag[label_cineon[TAG_SOURCE_INPUT_SERIAL]] =
            to_string(source.input_serial, 32);

    if (is_valid(&source.input_pitch[0]) && is_valid(&source.input_pitch[1]))
        info.tag[label_cineon[TAG_SOURCE_INPUT_PITCH]] =
            String_Util::join(List<String>() <<
                String_Util::float_to_string(source.input_pitch[0]) <<
                String_Util::float_to_string(source.input_pitch[1]),
                " ");

    if (is_valid(&source.gamma))
        info.tag[label_cineon[TAG_SOURCE_GAMMA]] =
            String_Util::float_to_string(source.gamma);

    // Film image tags.

    if (is_valid(&film.id) &&
        is_valid(&film.type) &&
        is_valid(&film.offset) &&
        is_valid(&film.prefix) &&
        is_valid(&film.count))
    {
        info.tag[label[Image_Tag::KEYCODE]] = Time::keycode_to_string(
            film.id, film.type, film.prefix, film.count, film.offset);
    }

    if (is_valid(film.format, 32))
    {
        info.tag[label_cineon[TAG_FILM_FORMAT]] = to_string(film.format, 32);
    }

    if (is_valid(&film.frame))
    {
        info.tag[label_cineon[TAG_FILM_FRAME]] =
            String_Util::int_to_string(film.frame);
    }
    
    if (is_valid(&film.frame_rate) && film.frame_rate >= speed_min)
    {
        info.seq.speed = Speed::float_to_speed(film.frame_rate);

        info.tag[label_cineon[TAG_FILM_FRAME_RATE]] =
            String_Util::float_to_string(film.frame_rate);
    }

    if (is_valid(film.frame_id, 32))
    {
        info.tag[label_cineon[TAG_FILM_FRAME_ID]] =
            to_string(film.frame_id, 32);
    }

    if (is_valid(film.slate, 200))
    {
        info.tag[label_cineon[TAG_FILM_SLATE]] = to_string(film.slate, 200);
    }

    debug();
}

void Header::save(
    File_Io & io,
    const Image_Io_Info & info,
    COLOR_PROFILE color_profile) throw (Error)
{

    //DJV_DEBUG("Header::save");

    // Information.

    file.image_offset = 2048;
    file.header_size = 1024;
    file.industry_header_size = 1024;
    file.user_header_size = 0;

    image.orient = ORIENT_LEFT_RIGHT_TOP_BOTTOM;

    image.channels = 3;

    if (COLOR_PROFILE_FILM_PRINT == color_profile)
    {
        image.channel[0].descriptor[1] = DESCRIPTOR_R_FILM_PRINT;
        image.channel[1].descriptor[1] = DESCRIPTOR_G_FILM_PRINT;
        image.channel[2].descriptor[1] = DESCRIPTOR_B_FILM_PRINT;
    }
    else
    {
        image.channel[0].descriptor[1] = DESCRIPTOR_L;
        image.channel[1].descriptor[1] = DESCRIPTOR_L;
        image.channel[2].descriptor[1] = DESCRIPTOR_L;
    }

    const int bit_depth = 10;

    for (int i = 0; i < image.channels; ++i)
    {
        image.channel[i].descriptor[0] = 0;
        image.channel[i].bit_depth = bit_depth;
        image.channel[i].size[0] = info.size.x;
        image.channel[i].size[1] = info.size.y;

        image.channel[i].low_data = 0;

        switch (bit_depth)
        {
            case 8:
                image.channel[i].high_data = 255;
                break;

            case 10:
                image.channel[i].high_data = 1023;
                break;

            case 12:
                image.channel[i].high_data = 4095;
                break;

            case 16:
                image.channel[i].high_data = 65535;
                break;
        }
    }

    image.interleave = 0;
    image.packing = 5;
    image.data_sign = 0;
    image.data_sense = 0;
    image.line_padding = 0;
    image.channel_padding = 0;

    // File image tags.

    const List<String> & label = Image_Tag::label_tag();
    const List<String> & label_cineon = label_tag();
    String tmp;

    String_Util::cstring(info.file_name, file.name, 100, false);
    String_Util::cstring(
        info.tag[label[Image_Tag::TIME]], file.time, 24, false);

    // Source image tags.

    tmp = info.tag[label_cineon[TAG_SOURCE_OFFSET]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (2 == list.size())
        {
            source.offset[0] = String_Util::string_to_int(list[0]);
            source.offset[1] = String_Util::string_to_int(list[1]);
        }
    }

    tmp = info.tag[label_cineon[TAG_SOURCE_FILE]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.file, 100, false);
    }

    tmp = info.tag[label_cineon[TAG_SOURCE_TIME]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.time, 24, false);
    }

    tmp = info.tag[label_cineon[TAG_SOURCE_INPUT_DEVICE]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.input_device, 64, false);
    }

    tmp = info.tag[label_cineon[TAG_SOURCE_INPUT_MODEL]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.input_model, 32, false);
    }

    tmp = info.tag[label_cineon[TAG_SOURCE_INPUT_SERIAL]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.input_serial, 32, false);
    }

    tmp = info.tag[label_cineon[TAG_SOURCE_INPUT_PITCH]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (2 == list.size())
        {
            source.offset[0] = String_Util::string_to_int(list[0]);
            source.offset[1] = String_Util::string_to_int(list[1]);
        }
    }

    tmp = info.tag[label_cineon[TAG_SOURCE_GAMMA]];

    if (tmp.size())
    {
        source.gamma = static_cast<float>(String_Util::string_to_float(tmp));
    }

    // Film image tags.

    tmp = info.tag[label[Image_Tag::KEYCODE]];

    if (tmp.size())
    {
        int id = 0, type = 0, prefix = 0, count = 0, offset = 0;
        Time::string_to_keycode(tmp, id, type, prefix, count, offset);
        film.id = id;
        film.type = type;
        film.offset = offset;
        film.prefix = prefix;
        film.count = count;
    }

    tmp = info.tag[label_cineon[TAG_FILM_FORMAT]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, film.format, 32, false);
    }

    tmp = info.tag[label_cineon[TAG_FILM_FRAME]];

    if (tmp.size())
    {
        film.frame = String_Util::string_to_int(tmp);
    }

    tmp = info.tag[label_cineon[TAG_FILM_FRAME_RATE]];

    if (tmp.size())
    {
        film.frame_rate = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_cineon[TAG_FILM_FRAME_ID]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, film.frame_id, 32, false);
    }

    tmp = info.tag[label_cineon[TAG_FILM_SLATE]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, film.slate, 200, false);
    }

    // Write.

    debug();

    const bool endian = Memory::endian() != Memory::MSB;
    io.endian(endian);

    if (endian)
    {
        //DJV_DEBUG_PRINT("endian");
        this->endian();
        file.magic = magic[1];
    }
    else
    {
        file.magic = magic[0];
    }

    io.set(&file, sizeof(File));
    io.set(&image, sizeof(Image));
    io.set(&source, sizeof(Source));
    io.set(&film, sizeof(Film));
}

void Header::endian()
{
    Memory::endian(&file.image_offset, 1, 4);
    Memory::endian(&file.header_size, 1, 4);
    Memory::endian(&file.industry_header_size, 1, 4);
    Memory::endian(&file.user_header_size, 1, 4);
    Memory::endian(&file.size, 1, 4);

    for (uint i = 0; i < 8; ++i)
    {
        Memory::endian(image.channel[i].size, 2, 4);
        Memory::endian(&image.channel[i].low_data, 1, 4);
        Memory::endian(&image.channel[i].low_quantity, 1, 4);
        Memory::endian(&image.channel[i].high_data, 1, 4);
        Memory::endian(&image.channel[i].high_quantity, 1, 4);
    }

    Memory::endian(image.white, 2, 4);
    Memory::endian(image.red, 2, 4);
    Memory::endian(image.green, 2, 4);
    Memory::endian(image.blue, 2, 4);
    Memory::endian(&image.line_padding, 1, 4);
    Memory::endian(&image.channel_padding, 1, 4);

    Memory::endian(source.offset, 2, 4);
    Memory::endian(source.input_pitch, 2, 4);
    Memory::endian(&source.gamma, 1, 4);

    Memory::endian(&film.prefix, 1, 4);
    Memory::endian(&film.count, 1, 4);
    Memory::endian(&film.frame, 1, 4);
    Memory::endian(&film.frame_rate, 1, 4);
}

void Header::zero(int32_t * in)
{
    *((uint32_t *)in) = 0x80000000;
}

void Header::zero(float * in)
{
    *((uint32_t *)in) = 0x7F800000;
}

void Header::zero(char * in, int size)
{
    Memory::zero(in, size);
}

bool Header::is_valid(const uint8_t * in)
{
    return *in != 0xff;
}

bool Header::is_valid(const uint16_t * in)
{
    return *in != 0xffff;
}

namespace
{

//! \todo Catch uninitialized values.

const int32_t _int_max = 1000000;
const float _float_max = 1000000.0;
const char _char_min = 32;
const char _char_max = 126;

}

bool Header::is_valid(const uint32_t * in)
{
    return
        *in != 0xffffffff &&
        *in < uint32_t(_int_max);
}

bool Header::is_valid(const int32_t * in)
{
    return
        *in != int32_t(0x80000000) &&
        *in > -_int_max &&
        *in < _int_max;
}

bool Header::is_valid(const float * in)
{
    return
        *((uint32_t *)in) != 0x7F800000 &&
        *in > -_float_max &&
        *in < _float_max;
}

bool Header::is_valid(const char * in, int size)
{
    const char * p = in;
    const char * const end = p + size;

    for (; *p && p < end; ++p)
        if (*p < _char_min || *p > _char_max)
        {
            return false;
        }

    return size ? (in[0] != 0) : false;
}

String Header::to_string(const char * in, int size)
{
    const char * p = in;
    const char * const end = p + size;

    for (; *p && p < end; ++p)
        ;

    return String(in, p - in);
}

void Header::info_load(
    File_Io &       io,
    Image_Io_Info & info,
    bool &          film_print) throw (Error)
{
    Header header;
    header.load(io, info, film_print);
}

void Header::info_save(
    File_Io &             io,
    const Image_Io_Info & info,
    COLOR_PROFILE         color_profile) throw (Error)
{
    Header header;
    header.save(io, info, color_profile);
}

void Header::info_update(File_Io & io) throw (Error)
{
    const uint32_t size = static_cast<uint32_t>(io.position());
    io.position(20);
    io.set_u32(size);
}

String Header::debug(uint8_t in)
{
    return is_valid(&in) ? String_Util::int_to_string(in) : "[]";
}

String Header::debug(uint16_t in)
{
    return is_valid(&in) ? String_Util::int_to_string(in) : "[]";
}

String Header::debug(uint32_t in)
{
    return is_valid(&in) ? String_Util::int_to_string(in) : "[]";
}

String Header::debug(int32_t in)
{
    return is_valid(&in) ? String_Util::int_to_string(in) : "[]";
}

String Header::debug(float in)
{
    return is_valid(&in) ? String_Util::float_to_string(in) : "[]";
}

String Header::debug(const char * in, int size)
{
    return is_valid(in, size) ? to_string(in, size) : "[]";
}

namespace
{

const String debug_file =
    "File\n"
    "  Image offset = %%\n"
    "  Header size = %%\n"
    "  Industry header size = %%\n"
    "  User header size = %%\n"
    "  Size = %%\n"
    "  Version = %%\n"
    "  Name = %%\n"
    "  Time = %%\n";

const String debug_image =
    "Image\n"
    "  Orient = %%\n"
    "  Channels = %%\n"
    "  White = %% %%\n"
    "  Red = %% %%\n"
    "  Green = %% %%\n"
    "  Blue = %% %%\n"
    "  Label = %%\n"
    "  Interleave = %%\n"
    "  Packing = %%\n"
    "  Data sign = %%\n"
    "  Data sense = %%\n"
    "  Line padding = %%\n"
    "  Channel padding = %%\n";

const String debug_image_channel =
    "Image Channel #%%\n"
    "  Descriptor = %% %%\n"
    "  Bit depth = %%\n"
    "  Size = %% %%\n"
    "  Low data = %%\n"
    "  Low quantity = %%\n"
    "  High data = %%\n"
    "  High quantity = %%\n";

const String debug_source =
    "Source\n"
    "  Offset = %% %%\n"
    "  File = %%\n"
    "  Time = %%\n"
    "  Input device = %%\n"
    "  Input model = %%\n"
    "  Input serial = %%\n"
    "  Input pitch = %% %%\n"
    "  Gamma = %%\n";

const String debug_film =
    "Film\n"
    "  Id = %%\n"
    "  Type = %%\n"
    "  Offset = %%\n"
    "  Prefix = %%\n"
    "  Count = %%\n"
    "  Format = %%\n"
    "  Frame = %%\n"
    "  Frame rate = %%\n"
    "  Frame ID = %%\n"
    "  Slate = %%\n";

}

String Header::debug() const
{
    String out;

    out += String_Format(debug_file).
           arg(debug(file.image_offset)).
           arg(debug(file.header_size)).
           arg(debug(file.industry_header_size)).
           arg(debug(file.user_header_size)).
           arg(debug(file.size)).
           arg(debug(file.version, 8)).
           arg(debug(file.name, 100)).
           arg(debug(file.time, 24));

    out += String_Format(debug_image).
           arg(debug(image.orient)).
           arg(debug(image.channels)).
           arg(debug(image.white[0])).
           arg(debug(image.white[1])).
           arg(debug(image.red[0])).
           arg(debug(image.red[1])).
           arg(debug(image.green[0])).
           arg(debug(image.green[1])).
           arg(debug(image.blue[0])).
           arg(debug(image.blue[1])).
           arg(debug(image.label, 200)).
           arg(debug(image.interleave)).
           arg(debug(image.packing)).
           arg(debug(image.data_sign)).
           arg(debug(image.data_sense)).
           arg(debug(image.line_padding)).
           arg(debug(image.channel_padding));

    for (int i = 0; i < image.channels; ++i)
        out += String_Format(debug_image_channel).
               arg(i).
               arg(debug(image.channel[i].descriptor[0])).
               arg(debug(image.channel[i].descriptor[1])).
               arg(debug(image.channel[i].bit_depth)).
               arg(debug(image.channel[i].size[0])).
               arg(debug(image.channel[i].size[1])).
               arg(debug(image.channel[i].low_data)).
               arg(debug(image.channel[i].low_quantity)).
               arg(debug(image.channel[i].high_data)).
               arg(debug(image.channel[i].high_quantity));

    out += String_Format(debug_source).
           arg(debug(source.offset[0])).
           arg(debug(source.offset[1])).
           arg(debug(source.file, 100)).
           arg(debug(source.time, 24)).
           arg(debug(source.input_device, 64)).
           arg(debug(source.input_model, 32)).
           arg(debug(source.input_serial, 32)).
           arg(debug(source.input_pitch[0])).
           arg(debug(source.input_pitch[1])).
           arg(debug(source.gamma));

    out += String_Format(debug_film).
           arg(debug(film.id)).
           arg(debug(film.type)).
           arg(debug(film.offset)).
           arg(debug(film.prefix)).
           arg(debug(film.count)).
           arg(debug(film.format, 32)).
           arg(debug(film.frame)).
           arg(debug(film.frame_rate)).
           arg(debug(film.frame_id, 32)).
           arg(debug(film.slate, 200));

    return out;
}

} // djv_cineon

