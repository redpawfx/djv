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

//! \file djv_libquicktime_save.cpp

#include <djv_libquicktime_save.h>

#include <djv_gl_image.h>

namespace djv_libquicktime
{

//------------------------------------------------------------------------------
// djv_image_save
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_save()
{
    quicktime_init();

    return new Save;
}

}

//------------------------------------------------------------------------------
// Save::Options
//------------------------------------------------------------------------------

Save::Options::Options() :
    codec("jpeg")
{}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Codec";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Save::Save() :
    _f             (0),
    _frame         (0),
    _frame_duration(0)
{}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_options = _options;
    return out;
}

String Save::name() const
{
    return djv_libquicktime::name;
}

List<String> Save::extensions() const
{
    return djv_libquicktime::extensions;
}

bool Save::is_seq() const
{
    return false;
}

bool Save::option(const String & in, String * data)
{
    const List<String> & list = options();

    try
    {
        if (String_Util::compare_no_case(in, list[CODEC]))
        {
            *data >> _options.codec;
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
    const List<String> & list = options();

    String out;

    if (String_Util::compare_no_case(in, list[CODEC]))
    {
        out << _options.codec;
    }

    return out;
}

List<String> Save::options() const
{
    return label_options();
}

namespace
{

struct Codec_Info
{
    Codec_Info(const String & name) :
        p(lqt_find_video_codec_by_name(name.c_str()))
    {}

    ~Codec_Info()
    {
        if (p)
        {
            lqt_destroy_codec_info(p);
        }
    }

    lqt_codec_info_t ** p;
};

const String
error_codec = "Cannot set codec for: %%",
error_colormodel = "Cannot set color model for: %%";

} // namespace

void Save::open(const File & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::open");
    //DJV_DEBUG_PRINT("in = " << in);
    //DJV_DEBUG_PRINT("info = " << info);

    // Open the file.

    _f = lqt_open_write(in.get().c_str(), LQT_FILE_QT);

    if (! _f)
    {
        throw_error_open(name(), in);
    }

    Codec_Info codec_info(_options.codec);

    if (! codec_info.p)
    {
        throw_error(name(), String_Format(error_codec).arg(in));
    }

    _frame = 0;
    _frame_duration = info.seq.speed.duration;

    //! \todo Return value?

    lqt_set_video(
        _f,
        1,
        info.size.x,
        info.size.y,
        _frame_duration,
        info.seq.speed.scale,
        codec_info.p[0]);

    _info = Pixel_Data_Info();
    _info.file_name = in;
    _info.size = info.size;
    _info.pixel = info.pixel;

    static int cmodel_list [] =
    {
        BC_RGB888,
        BC_BGR888,
        BC_RGB161616,
        BC_RGBA8888,
        BC_RGBA16161616,
        LQT_COLORMODEL_NONE
    };

    const int cmodel = lqt_get_best_colormodel(_f, 0, cmodel_list);

    switch (cmodel)
    {
        case BC_BGR888:
            _info.bgr = true;

        case BC_RGB888:
            _info.pixel = Pixel::RGB_U8;
            break;

        case BC_RGB161616:
            _info.pixel = Pixel::RGB_U16;
            break;

        case BC_RGBA8888:
            _info.pixel = Pixel::RGBA_U8;
            break;

        case BC_RGBA16161616:
            _info.pixel = Pixel::RGBA_U16;
            break;

        default:
            throw_error(name(), String_Format(error_colormodel).arg(in));
    }

    lqt_set_cmodel(_f, 0, cmodel);

    //DJV_DEBUG_PRINT("info = " << _info);

    const List<String> & label_tag = Image_Tag::label_tag();
    String tag;

    tag = info.tag[label_tag[Image_Tag::CREATOR]];

    if (tag.size())
    {
        lqt_set_artist(_f, (char *)tag.c_str());
    }

    tag = info.tag[label_tag[Image_Tag::DESCRIPTION]];

    if (tag.size())
    {
        lqt_set_comment(_f, (char *)tag.c_str());
    }

    tag = info.tag[label_tag[Image_Tag::COPYRIGHT]];

    if (tag.size())
    {
        quicktime_set_copyright(_f, (char *)tag.c_str());
    }

    _image.set(_info);
}

void Save::close() throw (Error)
{
    //DJV_DEBUG("Save::close");

    if (_f)
    {
        quicktime_close(_f);
        _f = 0;
    }
}

namespace
{

// Convert our image data to an array of row pointers for libquicktime.

struct Rows
{
    Rows(const Pixel_Data * in) :
        p(new const unsigned char * [in->h() + 1])
    {
        for (int y = 0; y < in->h(); ++y)
        {
            p[y] = in->data(0, in->h() - 1 - y);
        }
    }

    ~Rows()
    {
        delete [] p;
    }

    const unsigned char ** p;
};

} // namespace

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::set");
    //DJV_DEBUG_PRINT("in = " << in);

    // Convert the image if necessary.

    const Pixel_Data * p = &in;

    if (in.info() != _info)
    {
        //DJV_DEBUG_PRINT("convert = " << _image);

        _image.zero();

        Gl_Image::copy(in, _image);

        p = &_image;
    }

    // Write the image.

    lqt_encode_video(
        _f,
        const_cast<unsigned char **>(Rows(p).p),
        0,
        _frame * _frame_duration);

    ++_frame;
}

} // djv_libquicktime

