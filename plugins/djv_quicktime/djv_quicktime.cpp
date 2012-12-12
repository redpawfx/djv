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

//! \file djv_quicktime.cpp

#include <djv_quicktime.h>

#include <djv_debug.h>
#include <djv_image_io.h>

namespace qt
{
#if defined(DJV_OSX)
#include <QuickTime/QTML.h>
#elif defined(DJV_WINDOWS)
#include <QTML.h>
#endif
}

namespace djv_quicktime
{

const List<String> & label_codec()
{
    static const List<String> data = List<String>() <<
        "Raw" <<
        "JPEG" <<
        "MJPEG-A" <<
        "MJPEG-B" <<
        "H263" <<
        "H264" <<
        "DVC-NTSC" <<
        "DVC-PAL";

    DJV_ASSERT(data.size() == _CODEC_SIZE);

    return data;
}

const List<String> & label_quality()
{
    static const List<String> data = List<String>() <<
        "Lossless" <<
        "Min" <<
        "Max" <<
        "Low" <<
        "Normal" <<
        "High";

    DJV_ASSERT(data.size() == _QUALITY_SIZE);

    return data;
}

Handle::Handle(qt::Handle in) :
    p(in)
{}

Handle::~Handle()
{
    if (p)
    {
        qt::DisposeHandle(p);
    }
}

void Handle::init(long in) throw (Error)
{
    p = qt::NewHandle(in);
}

CFStringRef::CFStringRef(const String & in) :
    p(qt::CFStringCreateWithCString(0, in.c_str(), 0))
{}

CFStringRef::~CFStringRef()
{
    qt::CFRelease(p);
}

namespace
{

int  _ref          = 0;
bool _init_qtml    = false;
bool _enter_movies = false;

const String
    error_init = "Cannot initialize (#%%)",
    error_version = "Incompatible version (#%%)";

} // namespace

void quicktime_init() throw (Error)
{
    ++_ref;

    if (_ref > 1)
    {
        return;
    }

    //DJV_DEBUG("quicktime_init");

    qt::OSErr err = qt::noErr;

# if defined(DJV_WINDOWS)

    //DJV_DEBUG_PRINT("initialize QTML");

    err = qt::InitializeQTML(0); //qt::kInitializeQTMLNoSoundFlag);

    if (err != qt::noErr)
    {
        Image_Io_Base::throw_error(name, String_Format(error_init).arg(err));
    }

    _init_qtml = true;

# endif // DJV_WINDOWS

    //DJV_DEBUG_PRINT("get version");

    long version = 0;
    err = qt::Gestalt(qt::gestaltQuickTime, &version);

    if (err != qt::noErr)
    {
        Image_Io_Base::throw_error(name, String_Format(error_version).arg(err));
    }

    //DJV_DEBUG_PRINT("version = " << version);

    //DJV_DEBUG_PRINT("enter movies");

    err = qt::EnterMovies();

    if (err != qt::noErr)
    {
        Image_Io_Base::throw_error(name, String_Format(error_init).arg(err));
    }

    _enter_movies = true;
}

void quicktime_del()
{
    --_ref;

    if (_ref)
    {
        return;
    }

    //DJV_DEBUG("quicktime_del");

    if (_enter_movies)
    {
        //DJV_DEBUG_PRINT("exit movies");

        qt::ExitMovies();

        _enter_movies = false;
    }

# if defined(DJV_WINDOWS)

    if (_init_qtml)
    {
        //DJV_DEBUG_PRINT("terminate QTML");

        qt::TerminateQTML();

        _init_qtml = false;
    }

# endif // DJV_WINDOWS
}

qt::CodecType quicktime_codec(CODEC in)
{
    switch (in)
    {
        case CODEC_RAW:
            return qt::kRawCodecType;

        case CODEC_JPEG:
            return qt::kJPEGCodecType;

        case CODEC_MJPEGA:
            return qt::kMotionJPEGACodecType;

        case CODEC_MJPEGB:
            return qt::kMotionJPEGBCodecType;

        case CODEC_H263:
            return qt::kH263CodecType;

        case CODEC_H264:
            return qt::kH264CodecType;

        case CODEC_DVCNTSC:
            return qt::kDVCNTSCCodecType;

        case CODEC_DVCPAL:
            return qt::kDVCPALCodecType;

        default:
            break;
    }

    return 0;
}

qt::CodecQ quicktime_quality(QUALITY in)
{
    switch (in)
    {
        case QUALITY_LOSSLESS:
            return qt::codecLosslessQuality;

        case QUALITY_MIN:
            return qt::codecMinQuality;

        case QUALITY_MAX:
            return qt::codecMaxQuality;

        case QUALITY_LOW:
            return qt::codecLowQuality;

        case QUALITY_NORMAL:
            return qt::codecNormalQuality;

        case QUALITY_HIGH:
            return qt::codecHighQuality;

        default:
            break;
    }

    return 0;
}

_DJV_STRING_OPERATOR_LABEL(CODEC, label_codec())
_DJV_STRING_OPERATOR_LABEL(QUALITY, label_quality())

} // djv_quicktime

