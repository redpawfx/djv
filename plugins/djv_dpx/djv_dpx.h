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

//! \file djv_dpx.h

#ifndef DJV_DPX_H
#define DJV_DPX_H

#include <djv_cineon.h>

//! \namespace djv_dpx
//!
//! This plugin provides support for the SMPTE Digital Picture Exchange (DPX)
//! image file format. DPX is a specialized image file format for working with
//! motion picture film. DPX is the successor to the Cineon file format with
//! support for additional image and meta data.
//!
//! Supports:
//!
//! - Images: 10-bit RGB type "A" packing (the most common variety); 8-bit,
//!   16-bit, Luminance, RGB, RGBA
//! - Interleaved channels only
//!
//! References:
//!
//! - SMPTE, "SMPTE 268M-2003"
//!   http://www.smpte.org
//! - Cinesite, "Conversion of 10-bit Log Film Data To 8-bit Linear or Video
//!   Data"

namespace djv_dpx
{
using namespace djv;

static const String name = "DPX";

static const List<String> extensions = List<String>() <<
    ".dpx";

//! File format version.

enum VERSION
{
    VERSION_1_0,
    VERSION_2_0,

    _VERSION_SIZE
};

//! Get the file format version labels.

const List<String> & label_version();

//! File type.

enum TYPE
{
    TYPE_AUTO,
    TYPE_U10,

    _TYPE_SIZE
};

//! Get the file type labels.

const List<String> & label_type();

//! File endian.

enum ENDIAN
{
    ENDIAN_AUTO,
    ENDIAN_MSB,
    ENDIAN_LSB,

    _ENDIAN_SIZE
};

//! Get the file endian labels.

const List<String> & label_endian();

//! Image tags.

enum TAG
{
    TAG_SOURCE_OFFSET,
    TAG_SOURCE_CENTER,
    TAG_SOURCE_SIZE,
    TAG_SOURCE_FILE,
    TAG_SOURCE_TIME,
    TAG_SOURCE_INPUT_DEVICE,
    TAG_SOURCE_INPUT_SERIAL,
    TAG_SOURCE_BORDER,
    TAG_SOURCE_PIXEL_ASPECT,
    TAG_SOURCE_SCAN_SIZE,
    TAG_FILM_FORMAT,
    TAG_FILM_FRAME,
    TAG_FILM_SEQUENCE,
    TAG_FILM_HOLD,
    TAG_FILM_FRAME_RATE,
    TAG_FILM_SHUTTER,
    TAG_FILM_FRAME_ID,
    TAG_FILM_SLATE,
    TAG_TV_INTERLACE,
    TAG_TV_FIELD,
    TAG_TV_VIDEO_SIGNAL,
    TAG_TV_SAMPLE_RATE,
    TAG_TV_FRAME_RATE,
    TAG_TV_TIME_OFFSET,
    TAG_TV_GAMMA,
    TAG_TV_BLACK_LEVEL,
    TAG_TV_BLACK_GAIN,
    TAG_TV_BREAK_POINT,
    TAG_TV_WHITE_LEVEL,
    TAG_TV_INTEGRATION_TIMES,

    _TAG_SIZE
};

//! Get the image tag labels.

const List<String> & label_tag();

String & operator >> (String &, VERSION &) throw (String);
String & operator >> (String &, TYPE &) throw (String);
String & operator >> (String &, ENDIAN &) throw (String);

String & operator << (String &, VERSION);
String & operator << (String &, TYPE);
String & operator << (String &, ENDIAN);

} // djv_dpx

#endif // DJV_DPX_H

