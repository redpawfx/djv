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

//! \file djv_openexr_load.cpp

#include <djv_openexr_load.h>

#include <djv_gl_image.h>

#include <ImfChannelList.h>
#include <ImfHeader.h>
#include <ImfInputFile.h>
#include <ImfRgbaYca.h>

#include <algorithm>

namespace djv_openexr
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
// Load::Options
//------------------------------------------------------------------------------

Load::Options::Options()  :
    color_profile(COLOR_PROFILE_GAMMA),
    gamma        (2.2),
    channels     (CHANNELS_GROUP_KNOWN)
{}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

const List<String> & Load::label_options()
{
    static const List<String> data = List<String>() <<
        "Color Profile" <<
        "Gamma" <<
        "Exposure" <<
        "Channels";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Load::Load() :
    _f(0)
{}

Load::~Load()
{
    _close();
}

void Load::init() throw (Error)
{
    openexr_init();
}

void Load::del()
{
    openexr_del();
}

Plugin * Load::copy() const
{
    Load * out = new Load;
    out->_options = _options;
    return out;
}

String Load::name() const
{
    return djv_openexr::name;
}

List<String> Load::extensions() const
{
    return djv_openexr::extensions;
}

bool Load::option(const String & in, String * data)
{
    const List<String> & list = options();

    try
    {
        if (String_Util::compare_no_case(in, list[COLOR_PROFILE_OPTION]))
        {
            *data >> _options.color_profile;
        }
        else if (String_Util::compare_no_case(in, list[GAMMA_OPTION]))
        {
            *data >> _options.gamma;
        }
        else if (String_Util::compare_no_case(in, list[EXPOSURE_OPTION]))
        {
            *data >> _options.exposure;
        }
        else if (String_Util::compare_no_case(in, list[CHANNELS_OPTION]))
        {
            *data >> _options.channels;
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

    if (String_Util::compare_no_case(in, list[COLOR_PROFILE_OPTION]))
    {
        out << _options.color_profile;
    }
    else if (String_Util::compare_no_case(in, list[GAMMA_OPTION]))
    {
        out << _options.gamma;
    }
    else if (String_Util::compare_no_case(in, list[EXPOSURE_OPTION]))
    {
        out << _options.exposure;
    }
    else if (String_Util::compare_no_case(in, list[CHANNELS_OPTION]))
    {
        out << _options.channels;
    }

    return out;
}

List<String> Load::options() const
{
    return label_options();
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

void Load::_open(const String & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    _close();

    try
    {
        _f = new Imf::InputFile(in.c_str());

        // Get file information.

        _display_window = imf_to_box(_f->header().displayWindow());
        _data_window = imf_to_box(_f->header().dataWindow());

        //DJV_DEBUG_PRINT("display window = " << _display_window);
        //DJV_DEBUG_PRINT("data window = " << _data_window);

        Pixel_Data_Info _info;
        _info.size = _display_window.size;
        _info.mirror.y = true;

        _layers = layer(_f->header().channels(), _options.channels);
        info.layer_size(_layers.size());

        for (size_t i = 0; i < _layers.size(); ++i)
        {
            const V2i sampling(
                _layers[i].channel[0].sampling.x,
                _layers[i].channel[0].sampling.y);

            Pixel_Data_Info tmp = _info;
            tmp.file_name = in;
            tmp.layer_name = _layers[i].name;
            tmp.size /= sampling;

            if (! Pixel::pixel(
                Pixel::format(static_cast<int>(_layers[i].channel.size())),
                _layers[i].channel[0].type,
                &tmp.pixel))
            {
                throw_error_unsupported(name(), in);
            }

            info[i] = tmp;
        }

        // Get image tags.

        tag_load(_f->header(), info);
    }
    catch (const std::exception & error)
    {
        throw Error(name(), error.what());
    }
}

void Load::_close()
{
    delete _f;
    _f = 0;
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::load");
    //DJV_DEBUG_PRINT("frame = " << frame);

    try
    {
        // Open the file.

        const String file_name =
            _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());

        //DJV_DEBUG_PRINT("file name = " << file_name);

        Image_Io_Info info;
        _open(file_name, info);

        if (frame.layer < 0 ||
            frame.layer >= static_cast<int>(_layers.size()))
        {
            throw_error_read(name(), file_name);
        }

        Pixel_Data_Info _info = info[frame.layer];
        _info.size = _data_window.size;

        //! Set the image tags.
        
        image.tag = info.tag;

        //! Set the color profile.

        if (_options.color_profile != COLOR_PROFILE_NONE)
        {
            //DJV_DEBUG_PRINT("color profile");

            switch (_options.color_profile)
            {
                case COLOR_PROFILE_GAMMA:

                    //DJV_DEBUG_PRINT("gamma = " << _options.gamma);

                    image.color_profile.type = Color_Profile::GAMMA;

                    break;

                case COLOR_PROFILE_EXPOSURE:
                    image.color_profile.type = Color_Profile::EXPOSURE;
                    break;

                default:
                    break;
            }

            image.color_profile.gamma    = _options.gamma;
            image.color_profile.exposure = _options.exposure;
        }
        else
        {
            image.color_profile = Color_Profile();
        }
        
        // Read the file.

        Pixel_Data * data = frame.proxy ? &_tmp : &image;
        data->set(_info);

        const bool flip = Imf::DECREASING_Y == _f->header().lineOrder();

        //DJV_DEBUG_PRINT("flip = " << flip);

        const int channels = Pixel::channels(_info.pixel);
        const int bytes    = Pixel::channel_bytes(_info.pixel);
        const V2i sampling = _layers[frame.layer].channel[0].sampling;
        
        Imf::FrameBuffer frame_buffer;

        for (int c = 0; c < channels; ++c)
        {
            const String & channel = _layers[frame.layer].channel[c].name;

            //DJV_DEBUG_PRINT("channel = " << channel);

            frame_buffer.insert(
                channel.c_str(),
                Imf::Slice(
                    pixel_type_to_imf(Pixel::type(data->pixel())),
                    (char *)data->data() -
                    (_data_window.y * _info.size.x * channels * bytes) -
                    (_data_window.x * channels * bytes) +
                    c * bytes,
                    channels * bytes,
                    _info.size.x * channels * bytes,
                    sampling.x,
                    sampling.y,
                    0.0));
        }

        _f->setFrameBuffer(frame_buffer);

        if (flip)
        {
            for (
                int y = 0;
                y < _info.size.y * sampling.y;
                y += sampling.y)
            {
                _f->readPixels(
                    _data_window.y + (_info.size.y * sampling.y - 1 - y));
            }
        }
        else
        {
            _f->readPixels(
                _data_window.y,
                _data_window.y + _info.size.y * sampling.y - 1);
        }

        if (_display_window != _data_window)
        {
            //DJV_DEBUG_PRINT("display window");

            //! \todo Is there a more efficient way of reading the file's
            //! display/data window so we don't need this extra copy?

            Pixel_Data tmp = *data;

            _info.size = _display_window.size;
            data->set(_info);

            Gl_Image_Options options;
            options.xform.position =
                _data_window.position - _display_window.position;
            
            Gl_Image::copy(tmp, *data, options);
        }

        if (frame.proxy)
        {
            //DJV_DEBUG_PRINT("proxy");

            _info.size = Pixel_Data::proxy_scale(_info.size, frame.proxy);
            _info.proxy = frame.proxy;
            image.set(_info);

            Pixel_Data::proxy_scale(_tmp, &image, frame.proxy);
        }
    }
    catch (const std::exception & error)
    {
        throw Error(name(), error.what());
    }

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_openexr
