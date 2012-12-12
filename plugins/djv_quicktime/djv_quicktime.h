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

//! \file djv_quicktime.h

#ifndef DJV_QUICKTIME_H
#define DJV_QUICKTIME_H

#include <djv_error.h>
#include <djv_gl.h>
#include <djv_string.h>

namespace qt
{
#if defined(DJV_OSX)
#include <QuickTime/QuickTime.h>
#elif defined(DJV_WINDOWS)
#include <Movies.h>
#endif
#define qt_fixed1 ((qt::Fixed)0x00010000L) //! \todo Is this still necessary?
#undef check //! \todo QuickTime defines "check"?
}

//! \namespace djv_quicktime
//!
//! This plugin supports the Apple QuickTime movie file format.
//!
//! Requires:
//!
//! - QuickTime - http://www.apple.com/quicktime
//!
//! Supports:
//!
//! - Only available for Apple OS X and Microsoft Windows 32-bit builds
//! - Images: 8-bit RGBA
//! - File compression

namespace djv_quicktime
{
using namespace djv;

static const String name = "QuickTime";

static const List<String> extensions = List<String>() <<
   ".qt"  <<
   ".mov" <<
   ".avi" <<
   ".mp4";

//! Video codec.

enum CODEC
{
    CODEC_RAW,
    CODEC_JPEG,
    CODEC_MJPEGA,
    CODEC_MJPEGB,
    CODEC_H263,
    CODEC_H264,
    CODEC_DVCNTSC,
    CODEC_DVCPAL,

    _CODEC_SIZE
};

//! Get the video codec labels.

const List<String> & label_codec();

//! Compression quality.

enum QUALITY
{
    QUALITY_LOSSLESS,
    QUALITY_MIN,
    QUALITY_MAX,
    QUALITY_LOW,
    QUALITY_NORMAL,
    QUALITY_HIGH,

    _QUALITY_SIZE
};

//! Get the compression quality labels.

const List<String> & label_quality();

//! This class provides a generic QuickTime handle.

class Handle
{
public:

    Handle(qt::Handle = 0);

    ~Handle();

    void init(long) throw (Error);

    qt::Handle p;
};

//! This class provides a QuickTime string handle.

class CFStringRef
{
public:

    CFStringRef(const String &);

    ~CFStringRef();

    qt::CFStringRef p;
};

//! Initialize QuickTime.

void quicktime_init() throw (Error);

//! De-initialize QuickTime.

void quicktime_del();

//! Convert to a QuickTime code.

qt::CodecType quicktime_codec(CODEC);

//! Convert to a QuickTime quality.

qt::CodecQ quicktime_quality(QUALITY);

String & operator >> (String &, CODEC &) throw (String);
String & operator >> (String &, QUALITY &) throw (String);

String & operator << (String &, CODEC);
String & operator << (String &, QUALITY);

} // djv_quicktime

#endif // DJV_QUICKTIME_H

