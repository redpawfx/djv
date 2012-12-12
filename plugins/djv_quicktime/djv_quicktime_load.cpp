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

//! \file djv_quicktime_load.cpp

#include <djv_quicktime_load.h>

namespace qt
{
#if defined(DJV_WINDOWS)
#include <CFString.h>
#include <ImageCompression.h>
#endif
}

namespace djv_quicktime
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
    _start_frame(0),
    _movie(0),
    _gworld(0)
{}

Load::~Load()
{
    _close();
}

void Load::init() throw (Error)
{
    quicktime_init();
}

void Load::del()
{
    quicktime_del();
}

Plugin * Load::copy() const
{
    Load * out = new Load;
    out->_options = _options;
    return out;
}

String Load::name() const
{
    return djv_quicktime::name;
}

List<String> Load::extensions() const
{
    return djv_quicktime::extensions;
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
        if (String_Util::compare_no_case(in, list[START_FRAME_OPTION]))
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

    if (String_Util::compare_no_case(in, list[START_FRAME_OPTION]))
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

const qt::OSType media_list [] = { qt::VisualMediaCharacteristic };
const int media_list_size = 1;

/*long _frame_count(qt::Movie in)
{
  long out = 0;

  for (qt::TimeValue time = 0; time >= 0; ++out)
  {
    qt::TimeValue tmp = 0;
    qt::GetMovieNextInterestingTime(in, qt::nextTimeStep, media_list_size,
      media_list, time, qt_fixed1, &tmp, 0);
    time = tmp;
  }

  return out;
}*/

qt::TimeValue _frame_duration(qt::Movie in)
{
    qt::TimeValue out = 0;

    qt::GetMovieNextInterestingTime(
        in,
        qt::nextTimeStep,
        media_list_size,
        media_list,
        out,
        qt_fixed1,
        0,
        &out);

    return out;
}

const String
    error_open = "Cannot open: %% (#%%)",
    error_gworld = "Cannot create GWorld: %% (#%%)";

} // namespace

void Load::open(const File & in, Image_Io_Info & info) throw (Error)
{
    //DJV_DEBUG("Load::open");
    //DJV_DEBUG_PRINT("in = " << in);

    _close();

    // Open the file.

    CFStringRef cf_string_ref(File_Util::path_absolute(in));

    qt::QTNewMoviePropertyElement prop_list [10];
    qt::ItemCount prop_size = 0;

    prop_list[prop_size].propClass = qt::kQTPropertyClass_DataLocation;
    prop_list[prop_size].propID =
        qt::kQTDataLocationPropertyID_CFStringNativePath;
    //qt::kQTDataLocationPropertyID_CFStringPosixPath;
    prop_list[prop_size].propValueSize = sizeof(cf_string_ref.p);
    prop_list[prop_size].propValueAddress = (void *)&cf_string_ref.p;
    prop_list[prop_size].propStatus = 0;
    ++prop_size;

    qt::Boolean unresolved = true;
    prop_list[prop_size].propClass = qt::kQTPropertyClass_MovieInstantiation;
    prop_list[prop_size].propID =
        qt::kQTMovieInstantiationPropertyID_DontAskUnresolvedDataRefs;
    prop_list[prop_size].propValueSize = sizeof(unresolved);
    prop_list[prop_size].propValueAddress = &unresolved;
    prop_list[prop_size].propStatus = 0;
    ++prop_size;

    qt::Boolean active = true;
    prop_list[prop_size].propClass = qt::kQTPropertyClass_NewMovieProperty;
    prop_list[prop_size].propID = qt::kQTNewMoviePropertyID_Active;
    prop_list[prop_size].propValueSize = sizeof(active);
    prop_list[prop_size].propValueAddress = &active;
    prop_list[prop_size].propStatus = 0;
    ++prop_size;

    qt::Boolean no_interact = true;
    prop_list[prop_size].propClass = qt::kQTPropertyClass_NewMovieProperty;
    prop_list[prop_size].propID =
        qt::kQTNewMoviePropertyID_DontInteractWithUser;
    prop_list[prop_size].propValueSize = sizeof(no_interact);
    prop_list[prop_size].propValueAddress = &no_interact;
    prop_list[prop_size].propStatus = 0;
    ++prop_size;

    qt::QTVisualContextRef visual = 0;
    prop_list[prop_size].propClass = qt::kQTPropertyClass_Context;
    prop_list[prop_size].propID = qt::kQTContextPropertyID_VisualContext;
    prop_list[prop_size].propValueSize = sizeof(qt::QTVisualContextRef);
    prop_list[prop_size].propValueAddress = &visual;
    prop_list[prop_size].propStatus = 0;
    ++prop_size;

    qt::QTAudioContextRef audio = 0;
    prop_list[prop_size].propClass = qt::kQTPropertyClass_Context;
    prop_list[prop_size].propID = qt::kQTContextPropertyID_AudioContext;
    prop_list[prop_size].propValueSize = sizeof(qt::QTAudioContextRef);
    prop_list[prop_size].propValueAddress = &audio;
    prop_list[prop_size].propStatus = 0;
    ++prop_size;

    qt::OSErr err = qt::NewMovieFromProperties(
        prop_size,
        prop_list,
        0,
        0,
        &_movie);

    if (err != qt::noErr)
    {
        throw_error(name(), String_Format(error_open).arg(in).arg(err));
    }

    // Get the file information.

    _info.file_name = in;

    qt::Rect rect;
    qt::GetMovieBox(_movie, &rect);
    _info.size = V2i(rect.right - rect.left, rect.bottom - rect.top);
    _info.mirror.y = true;
    _info.pixel = Pixel::RGBA_U8;
    //_info.bgr = true;

    const qt::TimeScale time_scale = qt::GetMovieTimeScale(_movie);
    const qt::TimeValue time_duration = qt::GetMovieDuration(_movie);
    //const long frame_count = _frame_count(_movie);
    const qt::TimeValue frame_duration = _frame_duration(_movie);
    //const TimeValue frame_duration = time_duration / frame_count;
    _start_frame = _options.start_frame;

    //DJV_DEBUG_PRINT("time scale = " << static_cast<int>(time_scale));
    //DJV_DEBUG_PRINT("time duration = " << static_cast<int>(time_duration));
    //DJV_DEBUG_PRINT("frame count = " << static_cast<int>(frame_count));
    //DJV_DEBUG_PRINT("frame duration = " << static_cast<int>(frame_duration));
    //DJV_DEBUG_PRINT("start frame = " << _start_frame);

    _info.seq = Seq(
        _start_frame,
        frame_duration ?
        (_start_frame + (time_duration / frame_duration - 1)) :
        _start_frame,
        0,
        Speed(time_scale, frame_duration));

    //DJV_DEBUG_PRINT("time = " << _info.seq);

    info = _info;
    
    //! Allocate the temporary buffer.
    
    _tmp.set(_info);

    //! \todo Is this still necessary?

    unsigned long qt_format =
#   if defined(DJV_OSX)
        qt::k32ARGBPixelFormat;
#   else
        qt::k32RGBAPixelFormat;
#   endif

    err = qt::NewGWorldFromPtr(
        &_gworld,
        qt_format,
        &rect,
        0,
        0,
        0,
        (char *)_tmp.data(0, 0),
        _info.size.x * 4);

    if (err != qt::noErr)
    {
        throw_error(name(), String_Format(error_gworld).arg(in).arg(err));
    }

    qt::SetMovieGWorld(_movie, _gworld, 0);
}

void Load::_close()
{
    //DJV_DEBUG("Load::_close");

    if (_movie)
    {
        qt::DisposeMovie(_movie);
        
        _movie = 0;
    }

    if (_gworld)
    {
        qt::DisposeGWorld(_gworld);
        
        _gworld = 0;
    }
}

void Load::load(Image & image, const Image_Io_Frame_Info & frame) throw (Error)
{
    //DJV_DEBUG("Load::load");
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

    // Read the image.
    
    const qt::TimeValue time = _frame * _info.seq.speed.duration;
    //DJV_DEBUG_PRINT("time = " << static_cast<int>(time));
    qt::SetMovieTimeValue(_movie, time);
    qt::MoviesTask(_movie, 0);
    qt::UpdateMovie(_movie);

    Pixel_Data_Info info = _info;

    if (frame.proxy)
    {
        info.size  = Pixel_Data::proxy_scale(info.size, frame.proxy);
        info.proxy = frame.proxy;
        
        image.set(info);

        Pixel_Data::proxy_scale(_tmp, &image, frame.proxy);
    }
    else
    {
        image.set(_tmp);
    }
    
    //! \todo ARGB -> RGB

#   if defined(DJV_OSX)

    uint32_t * p = reinterpret_cast<uint32_t *>(image.data());
    
    const uint32_t * const end = p + info.size.x * info.size.y;

    switch (Memory::endian())
    {
        case Memory::MSB:
        
            for (; p < end; ++p)
            {
                *p = (*p) << 8;
            }

            break;

        case Memory::LSB:
        
            for (; p < end; ++p)
            {
                *p = (*p) >> 8;
            }

            break;
    }

#   endif

    //DJV_DEBUG_PRINT("image = " << image);
}

} // djv_quicktime

