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

//! \file djv_openexr_save.cpp

#include <djv_openexr_save.h>

#include <djv_gl_image.h>

#include <ImfChannelList.h>
#include <ImfCompressionAttribute.h>
#include <ImfHeader.h>
#include <ImfOutputFile.h>

namespace djv_openexr
{

//------------------------------------------------------------------------------
// djv_image_save
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_save()
{
    return new Save;
}

}

//------------------------------------------------------------------------------
// Save::Options
//------------------------------------------------------------------------------

Save::Options::Options() :
    compression(COMPRESSION_NONE)
{}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

const List<String> & Save::label_options()
{
    static const List<String> data = List<String>() <<
        "Compression";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Save::Save() :
    _f(0)
{}

Save::~Save()
{
    _close();
}

void Save::init() throw (Error)
{
    openexr_init();
}

void Save::del()
{
    openexr_del();
}

Plugin * Save::copy() const
{
    Save * out = new Save;
    out->_options = _options;
    return out;
}

String Save::name() const
{
    return djv_openexr::name;
}

List<String> Save::extensions() const
{
    return djv_openexr::extensions;
}

bool Save::option(const String & in, String * data)
{
    const List<String> & list = options();

    try
    {
        if (String_Util::compare_no_case(in, list[COMPRESSION_OPTION]))
        {
            *data >> _options.compression;
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

    if (String_Util::compare_no_case(in, list[COMPRESSION_OPTION]))
    {
        out << _options.compression;
    }

    return out;
}

List<String> Save::options() const
{
    return label_options();
}

void Save::open(const File & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::open");
    //DJV_DEBUG_PRINT("in = " << in);

    // File information.

    _file = in;

    if (info.seq.list.size() > 1)
    {
        _file.type(File::SEQ);
    }

    // Image information.

    _info = Pixel_Data_Info();
    _info.size = info.size;
    _info.mirror.y = true;

    Pixel::FORMAT format = Pixel::format(info.pixel);
    Pixel::TYPE type = Pixel::TYPE(0);

    switch (Pixel::type(info.pixel))
    {
        case Pixel::F32:
            type = Pixel::F32;
            break;

        default:
            type = Pixel::F16;
            break;
    }

    _info.pixel = Pixel::pixel(format, type);

    //DJV_DEBUG_PRINT("info = " << _info);

    _speed = info.seq.speed;

    // Initialize temporary image buffer.

    _tmp.set(_info);
}

void Save::_open(const String & in, const Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Save::_open");
    //DJV_DEBUG_PRINT("in = " << in);

    try
    {
        _close();

        // Set the header.

        Imf::Header header(info.size.x, info.size.y);

        switch (Pixel::channels(info.pixel))
        {
            case 1:
                _channels = List<String>() << "Y";
                break;

            case 2:
                _channels = List<String>() << "Y" << "A";
                break;

            case 3:
                _channels = List<String>() << "R" << "G" << "B";
                break;

            case 4:
                _channels = List<String>() << "R" << "G" << "B" << "A";
                break;
        }

        for (size_t i = 0; i < _channels.size(); ++i)
            header.channels().insert(
                _channels[i].c_str(),
                pixel_type_to_imf(Pixel::type(info.pixel)));

        Imf::CompressionAttribute compression;

        switch (_options.compression)
        {
            case COMPRESSION_NONE:
                compression = Imf::NO_COMPRESSION;
                break;

            case COMPRESSION_RLE:
                compression = Imf::RLE_COMPRESSION;
                break;

            case COMPRESSION_ZIPS:
                compression = Imf::ZIPS_COMPRESSION;
                break;

            case COMPRESSION_ZIP:
                compression = Imf::ZIP_COMPRESSION;
                break;

            case COMPRESSION_PIZ:
                compression = Imf::PIZ_COMPRESSION;
                break;

            case COMPRESSION_PXR24:
                compression = Imf::PXR24_COMPRESSION;
                break;

            case COMPRESSION_B44:
                compression = Imf::B44_COMPRESSION;
                break;

            case COMPRESSION_B44A:
                compression = Imf::B44A_COMPRESSION;
                break;

            default:
                break;
        }

        header.insert(Imf::CompressionAttribute::staticTypeName(), compression);

        // Set image tags.

        tag_save(info, header);

        // Open the file.

        _f = new Imf::OutputFile(in.c_str(), header);

    }
    catch (const std::exception & error)
    {
        throw Error(name(), error.what());
    }
}

void Save::_close()
{
    delete _f;
    _f = 0;
}

void Save::save(const Image & in, const Image_Io_Frame_Info & frame)
    throw (Error)
{
    //DJV_DEBUG("Save::save");
    //DJV_DEBUG_PRINT("in = " << in);

    try
    {
        // Open the file.

        Image_Io_Info info(_info);
        info.tag       = in.tag;
        info.seq.speed = _speed;

        _open(_file.get(frame.frame), info);

        // Convert the image.

        const Pixel_Data * p = &in;

        if (p->info() != _info)
        {
            //DJV_DEBUG_PRINT("convert = " << _tmp);

            _tmp.zero();

            Gl_Image::copy(in, _tmp);

            p = &_tmp;
        }

        // Write the file.

        const int w        = p->w();
        const int h        = p->h();
        const int channels = p->channels();
        const int bytes    = Pixel::channel_bytes(p->pixel());

        Imf::FrameBuffer frame_buffer;

        for (int c = 0; c < channels; ++c)
        {
            const String & channel = _channels[c];

            //DJV_DEBUG_PRINT("channel = " << channel);

            frame_buffer.insert(
                channel.c_str(),
                Imf::Slice(
                    pixel_type_to_imf(Pixel::type(p->pixel())),
                    (char *)p->data() + c * bytes,
                    channels * bytes,
                    w * channels * bytes,
                    1,
                    1,
                    0.0));
        }

        _f->setFrameBuffer(frame_buffer);

        _f->writePixels(h);

    }
    catch (const std::exception & err)
    {
        throw Error(name(), err.what());
    }

    _close();
}

} // djv_openexr

