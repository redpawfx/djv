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

//! \file djv_libquicktime_load.cpp

#include <djv_libquicktime_load.h>

#include <djv_assert.h>

namespace djv_libquicktime
{

//------------------------------------------------------------------------------
// djv_image_load
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_load()
{
    quicktime_init();

    return new Load;
}

}

//------------------------------------------------------------------------------
// Load::Options
//------------------------------------------------------------------------------

Load::Options::Options() :
    start_frame(0)
{}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

const List<String> & Load::label_options()
{
    static const List<String> data = List<String>() <<
        "Frame";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Load::Load() :
    _f          (0),
    _start_frame(0),
    _frame      (0)
{}

Load::~Load()
{
    _close();
}

Plugin * Load::copy() const
{
    Load * out = new Load;
    out->_options = _options;
    return out;
}

String Load::name() const
{
    return djv_libquicktime::name;
}

List<String> Load::extensions() const
{
    return djv_libquicktime::extensions;
}

bool Load::is_seq() const
{
    return false;
}

bool Load::option(const String & in, String * data)
{
    const List<String> & list = options();

    try
    {
        if (String_Util::compare_no_case(in, list[START_FRAME]))
        {
            *data >> _options.start_frame;
        }
    }
    catch (String)
    {
        return false;
    }

    return true;
}

String Load::option(const String & in) const
{
    const List<String> & list = options();

    String out;

    if (String_Util::compare_no_case(in, list[START_FRAME]))
    {
        out << _options.start_frame;
    }

    return out;
}

List<String> Load::options() const
{
    return label_options();
}

namespace
{

const String
  error_video = "No video tracks in: %%";

} // namespace

void Load::open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::open");
    //DJV_DEBUG_PRINT("in = " << in);

    _close();

    // Open the file.

    _f = lqt_open_read(in.get().c_str());

    if (! _f)
    {
        throw_error_open(name(), in);
    }

    if (! quicktime_has_video(_f))
    {
        throw_error(name(), String_Format(error_video).arg(in));
    }

    if (! quicktime_supported_video(_f, 0))
    {
        throw_error_unsupported(name(), in);
    }

    // Get file information.

    _info.file_name = in;

    _info.size = V2i(
        quicktime_video_width(_f, 0),
        quicktime_video_height(_f, 0));

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
            throw_error_unsupported(name(), in);
    }

    lqt_set_cmodel(_f, 0, cmodel);

    int time_constant = 0;
    const int time_scale = lqt_video_time_scale(_f, 0);
    const int64_t video_duration = lqt_video_duration(_f, 0);
    const int frame_duration = lqt_frame_duration(_f, 0, &time_constant);
    _start_frame = _options.start_frame;

    //DJV_DEBUG_PRINT("time scale = " << time_scale);
    //DJV_DEBUG_PRINT("video duration = " << video_duration);
    //DJV_DEBUG_PRINT("frame duration = " << frame_duration);
    //DJV_DEBUG_PRINT("time constant = " << time_constant);
    //DJV_DEBUG_PRINT("start frame = " << _start_frame);

    _info.seq = Seq(
        _start_frame,
        frame_duration ?
        (_start_frame + (video_duration / frame_duration - 1)) :
        _start_frame,
        0,
        Speed(time_scale, frame_duration));

    //DJV_DEBUG_PRINT("seq = " << _info.seq);

    // Get image tags.

    const List<String> & label_tag = Image_Tag::label_tag();
    char * tag = 0;

    lqt_get_artist(_f);

    if (tag)
    {
        _info.tag[label_tag[Image_Tag::CREATOR]] = tag;
    }

    tag = lqt_get_comment(_f);

    if (tag)
    {
        _info.tag[label_tag[Image_Tag::DESCRIPTION]] = tag;
    }

    tag = quicktime_get_copyright(_f);

    if (tag)
    {
        _info.tag[label_tag[Image_Tag::COPYRIGHT]] = tag;
    }

    info = _info;
}

void Load::_close()
{
    //DJV_DEBUG("Load::_close");

    if (_f)
    {
        quicktime_close(_f);
        _f = 0;
    }
}

namespace
{

struct Rows
{
    Rows(Pixel_Data * in) :
        p(new unsigned char * [in->h() + 1])
    {
        for (int y = 0; y < in->h(); ++y)
        {
            p[y] = reinterpret_cast<Pixel::U8_T *>(
                in->data(0, in->h() - 1 - y));
        }
    }

    ~Rows()
    {
        delete [] p;
    }

    unsigned char ** p;
};

} // namespace

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::get");
    //DJV_DEBUG_PRINT("frame = " << frame);

    image.color_profile = Color_Profile();
    image.tag = Image_Tag();

    int _frame = frame.frame;

    if (-1 == _frame)
    {
        _frame = 0;
    }
    else
    {
        _frame -= _start_frame;
    }

    Pixel_Data * data = frame.proxy ? &_tmp : &image;
    data->set(_info);

    //! \todo Is this still necessary?

    if (_frame != this->_frame + 1)
    {
        const uint64_t time = _frame * _info.seq.speed.duration;

        //DJV_DEBUG_PRINT("time = " << static_cast<int>(time));

        lqt_seek_video(_f, 0, time);
    }

    this->_frame = _frame;

    lqt_decode_video(_f, Rows(data).p, 0);

    if (frame.proxy)
    {
        Pixel_Data_Info info = _info;
        info.size = Pixel_Data::proxy_scale(info.size, frame.proxy);
        info.proxy = frame.proxy;
        image.set(info);

        Pixel_Data::proxy_scale(_tmp, &image, frame.proxy);
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_libquicktime

