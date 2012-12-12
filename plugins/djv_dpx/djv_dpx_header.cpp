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

//! \file djv_dpx_header.cpp

#include <djv_dpx_header.h>

#include <djv_cineon_header.h>

namespace djv_dpx
{

//------------------------------------------------------------------------------
// Header
//------------------------------------------------------------------------------

const char Header::magic [][5] =
{
    "SDPX",
    "XPDS"
};

Header::Header()
{
    Memory::set<uint8_t>(0xff, &file, sizeof(File));
    zero(file.version, 8);
    zero(file.name, 100);
    zero(file.time, 24);
    zero(file.creator, 100);
    zero(file.project, 200);
    zero(file.copyright, 200);

    Memory::set<uint8_t>(0xff, &image, sizeof(Image));

    Memory::set<uint8_t>(0xff, &source, sizeof(Source));
    zero(source.file, 100);
    zero(source.time, 24);
    zero(source.input_device, 32);
    zero(source.input_serial, 32);

    Memory::set<uint8_t>(0xff, &film, sizeof(Film));
    zero(film.id, 2);
    zero(film.type, 2);
    zero(film.offset, 2);
    zero(film.prefix, 6);
    zero(film.count, 4);
    zero(film.format, 32);
    zero(film.frame_id, 32);
    zero(film.slate, 100);

    Memory::set<uint8_t>(0xff, &tv, sizeof(Tv));
}

void Header::load(
    File_Io &       io,
    Image_Io_Info & info,
    bool &          film_print) throw (Error)
{

    //DJV_DEBUG("Header::load");

    // Read.

    io.get(&file, sizeof(File));

    //DJV_DEBUG_PRINT("magic = " << String((char *)&file.magic, 4));
    if (0 == Memory::compare(&file.magic, magic[0], 4))
    {
        info.endian = Memory::MSB;
    }
    else if (0 == Memory::compare(&file.magic, magic[1], 4))
    {
        info.endian = Memory::LSB;
    }
    else
    {
        Image_Io_Base::throw_error_unrecognized(name, io.file_name());
    }

    io.get(&image, sizeof(Image));
    io.get(&source, sizeof(Source));
    io.get(&film, sizeof(Film));
    io.get(&tv, sizeof(Tv));

    if (info.endian != Memory::endian())
    {
        //DJV_DEBUG_PRINT("endian");
        io.endian(true);
        endian();
    }

    // Information.

    if (image.elem_size != 1)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    info.size = V2i(image.size[0], image.size[1]);

    //DJV_DEBUG_PRINT("size = " << info.size);

    switch (image.orient)
    {
        case ORIENT_LEFT_RIGHT_TOP_BOTTOM:
            info.mirror.y = true;
            break;

        case ORIENT_RIGHT_LEFT_TOP_BOTTOM:
            info.mirror.x = true;
            break;

        case ORIENT_LEFT_RIGHT_BOTTOM_TOP:
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

    int pixel = -1;

    switch (image.elem[0].packing)
    {
        case PACK:
        {
            int channels = 0;

            switch (image.elem[0].descriptor)
            {
                case DESCRIPTOR_L:
                    channels = 1;
                    break;

                case DESCRIPTOR_RGB:
                    channels = 3;
                    break;

                case DESCRIPTOR_RGBA:
                    channels = 4;
                    break;
            }

            pixel = Pixel::pixel_(channels, image.elem[0].bit_depth, true);
        }
        break;

        case TYPE_A:
            switch (image.elem[0].descriptor)
            {
                case DESCRIPTOR_RGB:
                    switch (image.elem[0].bit_depth)
                    {
                        case 10:
                            pixel = Pixel::RGB_U10;
                            break;

                        case 16:
                        {
                            int channels = 0;

                            switch (image.elem[0].descriptor)
                            {
                                case DESCRIPTOR_L:
                                    channels = 1;
                                    break;

                                case DESCRIPTOR_RGB:
                                    channels = 3;
                                    break;

                                case DESCRIPTOR_RGBA:
                                    channels = 4;
                                    break;
                            }

                            pixel = Pixel::pixel_(
                                channels,
                                image.elem[0].bit_depth,
                                true);
                        }
                        break;
                    }

                    break;
            }

            break;
    }

    if (-1 == pixel)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    info.pixel = Pixel::PIXEL(pixel);

    //DJV_DEBUG_PRINT("pixel = " << info.pixel);

    switch (Pixel::bit_depth(info.pixel))
    {
        case 8:
        case 10:
            info.align = 4;
            break;
    }

    if (image.elem[0].encoding)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    if (is_valid(&image.elem[0].line_padding) && image.elem[0].line_padding)
    {
        Image_Io_Base::throw_error_unsupported(name, io.file_name());
    }

    film_print = TRANSFER_FILM_PRINT == image.elem[0].transfer;

    // File image tags.

    const List<String> & label = Image_Tag::label_tag();
    const List<String> & label_dpx = label_tag();

    if (is_valid(file.time, 24))
    {
        info.tag[label[Image_Tag::TIME]] = to_string(file.time, 24);
    }

    if (is_valid(file.creator, 100))
    {
        info.tag[label[Image_Tag::CREATOR]] = to_string(file.creator, 100);
    }

    if (is_valid(file.project, 200))
    {
        info.tag[label[Image_Tag::PROJECT]] = to_string(file.project, 200);
    }

    if (is_valid(file.copyright, 200))
    {
        info.tag[label[Image_Tag::COPYRIGHT]] = to_string(file.copyright, 200);
    }

    // Source image tags.

    if (is_valid(&source.offset[0]) && is_valid(&source.offset[1]))
        info.tag[label_dpx[TAG_SOURCE_OFFSET]] =
            String_Util::join(List<String>() <<
                String_Util::int_to_string(source.offset[0]) <<
                String_Util::int_to_string(source.offset[1]),
                " ");

    if (is_valid(&source.center[0]) && is_valid(&source.center[1]))
        info.tag[label_dpx[TAG_SOURCE_CENTER]] =
            String_Util::join(List<String>() <<
                String_Util::float_to_string(source.center[0]) <<
                String_Util::float_to_string(source.center[1]),
                " ");

    if (is_valid(&source.size[0]) && is_valid(&source.size[1]))
        info.tag[label_dpx[TAG_SOURCE_SIZE]] =
            String_Util::join(List<String>() <<
                String_Util::int_to_string(source.size[0]) <<
                String_Util::int_to_string(source.size[1]),
                " ");

    if (is_valid(source.file, 100))
    {
        info.tag[label_dpx[TAG_SOURCE_FILE]] = to_string(source.file, 100);
    }

    if (is_valid(source.time, 24))
    {
        info.tag[label[TAG_SOURCE_TIME]] = to_string(source.time, 24);
    }

    if (is_valid(source.input_device, 32))
        info.tag[label_dpx[TAG_SOURCE_INPUT_DEVICE]] =
            to_string(source.input_device, 32);

    if (is_valid(source.input_serial, 32))
        info.tag[label_dpx[TAG_SOURCE_INPUT_SERIAL]] =
            to_string(source.input_serial, 32);

    if (is_valid(&source.border[0]) && is_valid(&source.border[1]) &&
        is_valid(&source.border[2]) && is_valid(&source.border[3]))
        info.tag[label_dpx[TAG_SOURCE_BORDER]] =
            String_Util::join(List<String>() <<
                String_Util::int_to_string(source.border[0]) <<
                String_Util::int_to_string(source.border[1]) <<
                String_Util::int_to_string(source.border[2]) <<
                String_Util::int_to_string(source.border[3]),
                " ");

    if (is_valid(&source.pixel_aspect[0]) && is_valid(&source.pixel_aspect[1]))
        info.tag[label_dpx[TAG_SOURCE_PIXEL_ASPECT]] =
            String_Util::join(List<String>() <<
                String_Util::int_to_string(source.pixel_aspect[0]) <<
                String_Util::int_to_string(source.pixel_aspect[1]),
                " ");

    if (is_valid(&source.scan_size[0]) && is_valid(&source.scan_size[1]))
        info.tag[label_dpx[TAG_SOURCE_SCAN_SIZE]] =
            String_Util::join(List<String>() <<
                String_Util::float_to_string(source.scan_size[0]) <<
                String_Util::float_to_string(source.scan_size[1]),
                " ");

    // Film image tags.

    if (
        is_valid(film.id, 2) && is_valid(film.type, 2) &&
        is_valid(film.offset, 2) && is_valid(film.prefix, 6) &&
        is_valid(film.count, 4))
        info.tag[label[Image_Tag::KEYCODE]] = Time::keycode_to_string(
            String_Util::string_to_int(to_string(film.id, 2)),
            String_Util::string_to_int(to_string(film.type, 2)),
            String_Util::string_to_int(to_string(film.prefix, 6)),
            String_Util::string_to_int(to_string(film.count, 4)),
            String_Util::string_to_int(to_string(film.offset, 2)));

    if (is_valid(film.format, 32))
    {
        info.tag[label_dpx[TAG_FILM_FORMAT]] = to_string(film.format, 32);
    }

    if (is_valid(&film.frame))
        info.tag[label_dpx[TAG_FILM_FRAME]] =
            String_Util::int_to_string(film.frame);

    if (is_valid(&film.sequence))
        info.tag[label_dpx[TAG_FILM_SEQUENCE]] =
            String_Util::int_to_string(film.sequence);

    if (is_valid(&film.hold))
    {
        info.tag[label_dpx[TAG_FILM_HOLD]] =
            String_Util::int_to_string(film.hold);
    }

    if (is_valid(&film.frame_rate) &&
        film.frame_rate > djv_cineon::Header::speed_min)
    {
        info.seq.speed = Speed::float_to_speed(film.frame_rate);

        info.tag[label_dpx[TAG_FILM_FRAME_RATE]] =
            String_Util::float_to_string(film.frame_rate);
    }

    if (is_valid(&film.shutter))
        info.tag[label_dpx[TAG_FILM_SHUTTER]] =
            String_Util::float_to_string(film.shutter);

    if (is_valid(film.frame_id, 32))
    {
        info.tag[label_dpx[TAG_FILM_FRAME_ID]] = to_string(film.frame_id, 32);
    }

    if (is_valid(film.slate, 100))
    {
        info.tag[label_dpx[TAG_FILM_SLATE]] = to_string(film.slate, 100);
    }

    // TV image tags.

    if (is_valid(&tv.timecode))
        info.tag[label[Image_Tag::TIMECODE]] =
            Time::timecode_to_string(tv.timecode);

    if (is_valid(&tv.interlace))
        info.tag[label_dpx[TAG_TV_INTERLACE]] =
            String_Util::int_to_string(tv.interlace);

    if (is_valid(&tv.field))
    {
        info.tag[label_dpx[TAG_TV_FIELD]] =
            String_Util::int_to_string(tv.field);
    }

    if (is_valid(&tv.video_signal))
        info.tag[label_dpx[TAG_TV_VIDEO_SIGNAL]] =
            String_Util::int_to_string(tv.video_signal);

    if (is_valid(&tv.sample_rate[0]) && is_valid(&tv.sample_rate[1]))
        info.tag[label_dpx[TAG_TV_SAMPLE_RATE]] =
            String_Util::join(List<String>() <<
                String_Util::float_to_string(tv.sample_rate[0]) <<
                String_Util::float_to_string(tv.sample_rate[1]),
                " ");

    if (is_valid(&tv.frame_rate) &&
        tv.frame_rate > djv_cineon::Header::speed_min)
    {
        info.seq.speed = Speed::float_to_speed(tv.frame_rate);

        info.tag[label_dpx[TAG_TV_FRAME_RATE]] =
            String_Util::float_to_string(tv.frame_rate);
    }

    if (is_valid(&tv.time_offset))
        info.tag[label_dpx[TAG_TV_TIME_OFFSET]] =
            String_Util::float_to_string(tv.time_offset);

    if (is_valid(&tv.gamma))
    {
        info.tag[label_dpx[TAG_TV_GAMMA]] =
            String_Util::float_to_string(tv.gamma);
    }

    if (is_valid(&tv.black_level))
        info.tag[label_dpx[TAG_TV_BLACK_LEVEL]] =
            String_Util::float_to_string(tv.black_level);

    if (is_valid(&tv.black_gain))
        info.tag[label_dpx[TAG_TV_BLACK_GAIN]] =
            String_Util::float_to_string(tv.black_gain);

    if (is_valid(&tv.breakpoint))
        info.tag[label_dpx[TAG_TV_BREAK_POINT]] =
            String_Util::float_to_string(tv.breakpoint);

    if (is_valid(&tv.white_level))
        info.tag[label_dpx[TAG_TV_WHITE_LEVEL]] =
            String_Util::float_to_string(tv.white_level);

    if (is_valid(&tv.integration_times))
        info.tag[label_dpx[TAG_TV_INTEGRATION_TIMES]] =
            String_Util::float_to_string(tv.integration_times);

    // End.

    debug();

    if (file.image_offset)
    {
        io.position(file.image_offset);
    }
}

void Header::save(
    File_Io & io,
    const Image_Io_Info & info,
    ENDIAN endian,
    djv_cineon::COLOR_PROFILE color_profile,
    VERSION version) throw (Error)
{
    //DJV_DEBUG("Header::save");

    // Information.

    switch (version)
    {
        case VERSION_1_0:
            Memory::copy("V1.0", file.version, 4);
            break;

        case VERSION_2_0:
            Memory::copy("V2.0", file.version, 4);
            break;

        default:
            break;
    }

    file.image_offset = 2048;
    file.header_size = 2048 - 384;
    file.industry_header_size = 384;
    file.user_header_size = 0;
    file.size = 0;
    file.ditto_key = 1;

    image.elem_size = 1;
    image.size[0] = info.size.x;
    image.size[1] = info.size.y;
    image.orient = ORIENT_LEFT_RIGHT_TOP_BOTTOM;

    switch (info.pixel)
    {
        case Pixel::L_U8:
        case Pixel::L_U16:
        case Pixel::L_F16:
        case Pixel::L_F32:
            image.elem[0].descriptor = DESCRIPTOR_L;
            break;

        case Pixel::RGB_U8:
        case Pixel::RGB_U10:
        case Pixel::RGB_U16:
        case Pixel::RGB_F16:
        case Pixel::RGB_F32:
            image.elem[0].descriptor = DESCRIPTOR_RGB;
            break;

        case Pixel::RGBA_U8:
        case Pixel::RGBA_U16:
        case Pixel::RGBA_F16:
        case Pixel::RGBA_F32:
            image.elem[0].descriptor = DESCRIPTOR_RGBA;
            break;

        default:
            break;
    }

    switch (info.pixel)
    {
        case Pixel::RGB_U10:
            image.elem[0].packing = TYPE_A;
            break;

        default:
            break;
    }

    const int bit_depth = Pixel::bit_depth(info.pixel);
    image.elem[0].bit_depth = bit_depth;

    image.elem[0].data_sign = 0;
    image.elem[0].low_data = 0;

    switch (bit_depth)
    {
        case 8:
            image.elem[0].high_data = 255;
            break;

        case 10:
            image.elem[0].high_data = 1023;
            break;

        case 12:
            image.elem[0].high_data = 4095;
            break;

        case 16:
            image.elem[0].high_data = 65535;
            break;
    }

    if (djv_cineon::COLOR_PROFILE_RAW == color_profile)
    {
        image.elem[0].transfer = TRANSFER_LINEAR;
    }
    else
    {
        image.elem[0].transfer = TRANSFER_FILM_PRINT;
    }

    switch (version)
    {
        case VERSION_1_0:
            if (djv_cineon::COLOR_PROFILE_RAW == color_profile)
            {
                image.elem[0].colorimetric = COLORIMETRIC_1_0_USER;
            }
            else
            {
                image.elem[0].colorimetric = COLORIMETRIC_1_0_FILM_PRINT;
            }

            break;

        case VERSION_2_0:
            if (djv_cineon::COLOR_PROFILE_RAW == color_profile)
            {
                image.elem[0].colorimetric = COLORIMETRIC_2_0_USER;
            }
            else
            {
                image.elem[0].colorimetric = COLORIMETRIC_2_0_FILM_PRINT;
            }

            break;

        default:
            break;
    }

    image.elem[0].encoding = 0;
    image.elem[0].data_offset = 2048;
    image.elem[0].line_padding = 0;
    image.elem[0].elem_padding = 0;

    // File image tags.

    const List<String> & label = Image_Tag::label_tag();
    const List<String> & label_dpx = label_tag();
    String tmp;

    String_Util::cstring(info.file_name, file.name, 100, false);

    tmp = info.tag[label[Image_Tag::TIME]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, file.time, 24, false);
    }

    tmp = info.tag[label[Image_Tag::CREATOR]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, file.creator, 100, false);
    }

    tmp = info.tag[label[Image_Tag::PROJECT]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, file.project, 200, false);
    }

    tmp = info.tag[label[Image_Tag::COPYRIGHT]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, file.copyright, 200, false);
    }

    file.encryption_key = 0;

    // Source image tags.

    tmp = info.tag[label_dpx[TAG_SOURCE_OFFSET]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (2 == list.size())
        {
            source.offset[0] = String_Util::string_to_int(list[0]);
            source.offset[1] = String_Util::string_to_int(list[1]);
        }
    }

    tmp = info.tag[label_dpx[TAG_SOURCE_CENTER]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (2 == list.size())
        {
            source.center[0] =
                static_cast<float>(String_Util::string_to_float(list[0]));
            source.center[1] =
                static_cast<float>(String_Util::string_to_float(list[1]));
        }
    }

    tmp = info.tag[label_dpx[TAG_SOURCE_SIZE]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (2 == list.size())
        {
            source.size[0] = String_Util::string_to_int(list[0]);
            source.size[1] = String_Util::string_to_int(list[1]);
        }
    }

    tmp = info.tag[label_dpx[TAG_SOURCE_FILE]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.file, 100, false);
    }

    tmp = info.tag[label[TAG_SOURCE_TIME]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.time, 24, false);
    }

    tmp = info.tag[label_dpx[TAG_SOURCE_INPUT_DEVICE]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.input_device, 32, false);
    }

    tmp = info.tag[label_dpx[TAG_SOURCE_INPUT_SERIAL]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, source.input_serial, 32, false);
    }

    tmp = info.tag[label_dpx[TAG_SOURCE_BORDER]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (4 == list.size())
        {
            source.border[0] = String_Util::string_to_int(list[0]);
            source.border[1] = String_Util::string_to_int(list[1]);
            source.border[2] = String_Util::string_to_int(list[2]);
            source.border[3] = String_Util::string_to_int(list[3]);
        }
    }

    tmp = info.tag[label_dpx[TAG_SOURCE_PIXEL_ASPECT]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (2 == list.size())
        {
            source.pixel_aspect[0] = String_Util::string_to_int(list[0]);
            source.pixel_aspect[1] = String_Util::string_to_int(list[1]);
        }
    }

    tmp = info.tag[label_dpx[TAG_SOURCE_SCAN_SIZE]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (2 == list.size())
        {
            source.scan_size[0] =
                static_cast<float>(String_Util::string_to_float(list[0]));
            source.scan_size[1] =
                static_cast<float>(String_Util::string_to_float(list[1]));
        }
    }

    // Film image tags.

    tmp = info.tag[label[Image_Tag::KEYCODE]];

    if (tmp.size())
    {
        int id = 0, type = 0, prefix = 0, count = 0, offset = 0;
        Time::string_to_keycode(tmp, id, type, prefix, count, offset);
        String_Util::cstring(
            String_Util::int_to_string(id), film.id, 2, false);
        String_Util::cstring(
            String_Util::int_to_string(type), film.type, 2, false);
        String_Util::cstring(
            String_Util::int_to_string(offset), film.offset, 2, false);
        String_Util::cstring(
            String_Util::int_to_string(prefix), film.prefix, 6, false);
        String_Util::cstring(
            String_Util::int_to_string(count), film.count, 4, false);
    }

    tmp = info.tag[label_dpx[TAG_FILM_FORMAT]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, film.format, 32, false);
    }

    tmp = info.tag[label_dpx[TAG_FILM_FRAME]];

    if (tmp.size())
    {
        film.frame = String_Util::string_to_int(tmp);
    }

    tmp = info.tag[label_dpx[TAG_FILM_SEQUENCE]];

    if (tmp.size())
    {
        film.sequence = String_Util::string_to_int(tmp);
    }

    tmp = info.tag[label_dpx[TAG_FILM_HOLD]];

    if (tmp.size())
    {
        film.hold = String_Util::string_to_int(tmp);
    }

    tmp = info.tag[label_dpx[TAG_FILM_FRAME_RATE]];

    if (tmp.size())
    {
        film.frame_rate = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_FILM_SHUTTER]];

    if (tmp.size())
    {
        film.shutter = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_FILM_FRAME_ID]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, film.frame_id, 32, false);
    }

    tmp = info.tag[label_dpx[TAG_FILM_SLATE]];

    if (tmp.size())
    {
        String_Util::cstring(tmp, film.slate, 100, false);
    }

    // TV image tags.

    tmp = info.tag[label[Image_Tag::TIMECODE]];

    if (tmp.size())
    {
        tv.timecode = Time::string_to_timecode(tmp);
    }

    tmp = info.tag[label_dpx[TAG_TV_INTERLACE]];

    if (tmp.size())
    {
        tv.interlace = String_Util::string_to_int(tmp);
    }

    tmp = info.tag[label_dpx[TAG_TV_FIELD]];

    if (tmp.size())
    {
        tv.field = String_Util::string_to_int(tmp);
    }

    tmp = info.tag[label_dpx[TAG_TV_VIDEO_SIGNAL]];

    if (tmp.size())
    {
        tv.video_signal = String_Util::string_to_int(tmp);
    }

    tmp = info.tag[label_dpx[TAG_TV_SAMPLE_RATE]];

    if (tmp.size())
    {
        const List<String> list = String_Util::split(tmp, ' ');

        if (2 == list.size())
        {
            tv.sample_rate[0] =
                static_cast<float>(String_Util::string_to_float(list[0]));
            tv.sample_rate[1] =
                static_cast<float>(String_Util::string_to_float(list[1]));
        }
    }

    tmp = info.tag[label_dpx[TAG_TV_FRAME_RATE]];

    if (tmp.size())
    {
        tv.frame_rate = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_TV_TIME_OFFSET]];

    if (tmp.size())
    {
        tv.time_offset = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_TV_GAMMA]];

    if (tmp.size())
    {
        tv.gamma = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_TV_BLACK_LEVEL]];

    if (tmp.size())
    {
        tv.black_level = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_TV_BLACK_GAIN]];

    if (tmp.size())
    {
        tv.black_gain = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_TV_BREAK_POINT]];

    if (tmp.size())
    {
        tv.breakpoint = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_TV_WHITE_LEVEL]];

    if (tmp.size())
    {
        tv.white_level = static_cast<float>(String_Util::string_to_float(tmp));
    }

    tmp = info.tag[label_dpx[TAG_TV_INTEGRATION_TIMES]];

    if (tmp.size())
        tv.integration_times = static_cast<float>(
                                   String_Util::string_to_float(tmp));

    // Write.

    debug();

    Memory::ENDIAN file_endian = Memory::endian();

    if (ENDIAN_MSB == endian)
    {
        file_endian = Memory::MSB;
    }
    else if (ENDIAN_LSB == endian)
    {
        file_endian = Memory::LSB;
    }

    if (file_endian != Memory::endian())
    {
        //DJV_DEBUG_PRINT("endian");
        io.endian(true);
        this->endian();
    }

    Memory::copy(
        Memory::MSB == file_endian ? magic[0] : magic[1],
        &file.magic,
        4);

    io.set(&file, sizeof(File));
    io.set(&image, sizeof(Image));
    io.set(&source, sizeof(Source));
    io.set(&film, sizeof(Film));
    io.set(&tv, sizeof(Tv));
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
    return *in != 0xffffffff && *in < uint32_t(_int_max);
}

bool Header::is_valid(const float * in)
{
    return
        *((uint32_t *)in) != 0xffffffff &&
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
    File_Io &                 io,
    const Image_Io_Info &     info,
    ENDIAN                    endian,
    djv_cineon::COLOR_PROFILE color_profile,
    VERSION                   version) throw (Error)
{
    Header header;
    header.save(io, info, endian, color_profile, version);
}

void Header::info_update(File_Io & io) throw (Error)
{
    const uint32_t size = static_cast<uint32_t>(io.position());
    io.position(12);
    io.set_u32(size);
}

void Header::endian()
{
    Memory::endian(&file.image_offset, 1, 4);
    Memory::endian(&file.size, 1, 4);
    Memory::endian(&file.ditto_key, 1, 4);
    Memory::endian(&file.header_size, 1, 4);
    Memory::endian(&file.industry_header_size, 1, 4);
    Memory::endian(&file.user_header_size, 1, 4);
    Memory::endian(&file.encryption_key, 1, 4);

    Memory::endian(&image.orient, 1, 2);
    Memory::endian(&image.elem_size, 1, 2);
    Memory::endian(image.size, 2, 4);

    for (uint i = 0; i < 8; ++i)
    {
        Memory::endian(&image.elem[i].data_sign, 1, 4);
        Memory::endian(&image.elem[i].low_data, 1, 4);
        Memory::endian(&image.elem[i].low_quantity, 1, 4);
        Memory::endian(&image.elem[i].high_data, 1, 4);
        Memory::endian(&image.elem[i].high_quantity, 1, 4);
        Memory::endian(&image.elem[i].packing, 1, 2);
        Memory::endian(&image.elem[i].encoding, 1, 2);
        Memory::endian(&image.elem[i].data_offset, 1, 4);
        Memory::endian(&image.elem[i].line_padding, 1, 4);
        Memory::endian(&image.elem[i].elem_padding, 1, 4);
    }

    Memory::endian(source.offset, 2, 4);
    Memory::endian(source.center, 2, 4);
    Memory::endian(source.size, 2, 4);
    Memory::endian(source.border, 4, 2);
    Memory::endian(source.pixel_aspect, 2, 4);
    Memory::endian(source.scan_size, 2, 4);

    Memory::endian(&film.frame, 1, 4);
    Memory::endian(&film.sequence, 1, 4);
    Memory::endian(&film.hold, 1, 4);
    Memory::endian(&film.frame_rate, 1, 4);
    Memory::endian(&film.shutter, 1, 4);

    Memory::endian(&tv.timecode, 1, 4);
    Memory::endian(&tv.user_bits, 1, 4);
    Memory::endian(tv.sample_rate, 2, 4);
    Memory::endian(&tv.frame_rate, 1, 4);
    Memory::endian(&tv.time_offset, 1, 4);
    Memory::endian(&tv.gamma, 1, 4);
    Memory::endian(&tv.black_level, 1, 4);
    Memory::endian(&tv.black_gain, 1, 4);
    Memory::endian(&tv.breakpoint, 1, 4);
    Memory::endian(&tv.white_level, 1, 4);
    Memory::endian(&tv.integration_times, 1, 4);
}

namespace
{

const String debug_file =
    "File\n"
    "  Image offset = %%\n"
    "  Version = %%\n"
    "  File size = %%\n"
    "  Ditto key = %%\n"
    "  Header size = %%\n"
    "  Industry header size = %%\n"
    "  User header size = %%\n"
    "  Name = %%\n"
    "  Time = %%\n"
    "  Creator = %%\n"
    "  Project = %%\n"
    "  Copyright = %%\n"
    "  Encryption key = %%\n";

const String debug_image =
    "Image\n"
    "  Orient = %%\n"
    "  Element size = %%\n"
    "  Size = %% %%\n";

const String debug_image_element =
    "Image Element #%%\n"
    "  Data sign = %%\n"
    "  Low data = %%\n"
    "  Low quantity = %%\n"
    "  High data = %%\n"
    "  High quantity = %%\n"
    "  Descriptor = %%\n"
    "  Transfer = %%\n"
    "  Colorimetric = %%\n"
    "  Bit depth = %%\n"
    "  Packing = %%\n"
    "  Encoding = %%\n"
    "  Data offset = %%\n"
    "  Line padding = %%\n"
    "  Element padding = %%\n"
    "  Description = %%\n";

const String debug_source =
    "Source\n"
    "  Offset = %% %%\n"
    "  Center = %% %%\n"
    "  Size = %% %%\n"
    "  File = %%\n"
    "  Time = %%\n"
    "  Input device = %%\n"
    "  Input serial = %%\n"
    "  Border = %% %% %% %%\n"
    "  Pixel aspect = %% %%\n"
    "  Scan size = %% %%\n";

const String debug_film =
    "Film\n"
    "  ID = %%\n"
    "  Type = %%\n"
    "  Offset = %%\n"
    "  Prefix = %%\n"
    "  Count = %%\n"
    "  Format = %%\n"
    "  Frame = %%\n"
    "  Sequence = %%\n"
    "  Hold = %%\n"
    "  Frame rate = %%\n"
    "  Shutter = %%\n"
    "  Frame ID = %%\n"
    "  Slate = %%\n";

const String debug_tv =
    "TV\n"
    "  Timecode = %%\n"
    "  User bits = %%\n"
    "  Interlace = %%\n"
    "  Field = %%\n"
    "  Video signal = %%\n"
    "  Sample rate = %% %%\n"
    "  Frame rate = %%\n"
    "  Time offset = %%\n"
    "  Gamma = %%\n"
    "  Black level = %%\n"
    "  Black gain = %%\n"
    "  Breakpoint = %%\n"
    "  White level = %%\n"
    "  Integration times = %%\n";

} // namespace

String Header::debug() const
{
    String out;

    out += String_Format(debug_file).
           arg(debug(file.image_offset)).
           arg(debug(file.version, 8)).
           arg(debug(file.size)).
           arg(debug(file.ditto_key)).
           arg(debug(file.header_size)).
           arg(debug(file.industry_header_size)).
           arg(debug(file.user_header_size)).
           arg(debug(file.name, 100)).
           arg(debug(file.time, 24)).
           arg(debug(file.creator, 100)).
           arg(debug(file.project, 200)).
           arg(debug(file.copyright, 200)).
           arg(debug(file.encryption_key));

    out += String_Format(debug_image).
           arg(debug(image.orient)).
           arg(debug(image.elem_size)).
           arg(debug(image.size[0])).
           arg(debug(image.size[1]));

    for (int i = 0; i < image.elem_size; ++i)
        out += String_Format(debug_image_element).
               arg(i).
               arg(debug(image.elem[i].data_sign)).
               arg(debug(image.elem[i].low_data)).
               arg(debug(image.elem[i].low_quantity)).
               arg(debug(image.elem[i].high_data)).
               arg(debug(image.elem[i].high_quantity)).
               arg(debug(image.elem[i].descriptor)).
               arg(debug(image.elem[i].transfer)).
               arg(debug(image.elem[i].colorimetric)).
               arg(debug(image.elem[i].bit_depth)).
               arg(debug(image.elem[i].packing)).
               arg(debug(image.elem[i].encoding)).
               arg(debug(image.elem[i].data_offset)).
               arg(debug(image.elem[i].line_padding)).
               arg(debug(image.elem[i].elem_padding)).
               arg(debug(image.elem[i].description, 32));

    out += String_Format(debug_source).
           arg(debug(source.offset[0])).
           arg(debug(source.offset[1])).
           arg(debug(source.center[0])).
           arg(debug(source.center[1])).
           arg(debug(source.size[0])).
           arg(debug(source.size[1])).
           arg(debug(source.file, 100)).
           arg(debug(source.time, 24)).
           arg(debug(source.input_device, 32)).
           arg(debug(source.input_serial, 32)).
           arg(debug(source.border[0])).
           arg(debug(source.border[1])).
           arg(debug(source.border[2])).
           arg(debug(source.border[3])).
           arg(debug(source.pixel_aspect[0])).
           arg(debug(source.pixel_aspect[1])).
           arg(debug(source.scan_size[0])).
           arg(debug(source.scan_size[1]));

    out += String_Format(debug_film).
           arg(debug(film.id, 2)).
           arg(debug(film.type, 2)).
           arg(debug(film.offset, 2)).
           arg(debug(film.prefix, 6)).
           arg(debug(film.count, 4)).
           arg(debug(film.format, 32)).
           arg(debug(film.frame)).
           arg(debug(film.sequence)).
           arg(debug(film.hold)).
           arg(debug(film.frame_rate)).
           arg(debug(film.shutter)).
           arg(debug(film.frame_id, 32)).
           arg(debug(film.slate, 100));

    out += String_Format(debug_tv).
           arg(debug(tv.timecode)).
           arg(debug(tv.user_bits)).
           arg(debug(tv.interlace)).
           arg(debug(tv.field)).
           arg(debug(tv.video_signal)).
           arg(debug(tv.sample_rate[0])).
           arg(debug(tv.sample_rate[1])).
           arg(debug(tv.frame_rate)).
           arg(debug(tv.time_offset)).
           arg(debug(tv.gamma)).
           arg(debug(tv.black_level)).
           arg(debug(tv.black_gain)).
           arg(debug(tv.breakpoint)).
           arg(debug(tv.white_level)).
           arg(debug(tv.integration_times));

    return out;
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

String Header::debug(float in)
{
    return is_valid(&in) ? String_Util::float_to_string(in) : "[]";
}

String Header::debug(const char * in, int size)
{
    return is_valid(in, size) ? to_string(in, size) : "[]";
}

} // djv_dpx

