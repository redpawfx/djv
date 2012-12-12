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

//! \file djv_cineon.h

#ifndef DJV_CINEON_H
#define DJV_CINEON_H

#include <djv_file_io.h>
#include <djv_image_io.h>

//! \namespace djv_cineon
//!
//! This plugin provides support for the Kodak Cineon image file format. Cineon
//! is a specialized image file format for working with motion picture film.
//!
//! Supports:
//!
//! - Images: 10-bit RGB (the most common variety)
//! - Interleaved channels only
//!
//! References:
//!
//! - Kodak, "4.5 DRAFT - Image File Format Proposal for Digital Pictures"

namespace djv_cineon
{
using namespace djv;

static const String name = "Cineon";

static const List<String> extensions = List<String>() <<
    ".cin";

//! Color profile.

enum COLOR_PROFILE
{
    COLOR_PROFILE_AUTO,
    COLOR_PROFILE_RAW,
    COLOR_PROFILE_FILM_PRINT,

    _COLOR_PROFILE_SIZE
};

//! Get the color profile labels.

const List<String> & label_color_profile();

//! This struct provides film print options.
//!
//! - Black point = 0 - 1023, Default = 95
//! - White point = 0 - 1023, Default = 685
//! - Gamma = 0.01 - 4.0, Default = 1.7
//! - Soft clip = 0 - 50, Default = 0

struct Film_Print
{
    //! Constructor.

    Film_Print() :
        black(95),
        white(685),
        gamma(1.7)
    {}

    int    black;
    int    white;
    double gamma;
};

struct Film_Unprint
{
    //! Constructor.

    Film_Unprint() :
        black    (95),
        white    (685),
        gamma    (1.7),
        soft_clip(0)
    {}

    int    black;
    int    white;
    double gamma;
    int    soft_clip; // \todo Shouldn't soft clip be in Film_Print?
};

//! Convert bit depth.

enum CONVERT
{
    CONVERT_NONE,
    CONVERT_U8,
    CONVERT_U16,

    _CONVERT_SIZE
};

//! Get convert bit depth labels.

const List<String> & label_convert();

//! Image tags.

enum TAG
{
    TAG_SOURCE_OFFSET,
    TAG_SOURCE_FILE,
    TAG_SOURCE_TIME,
    TAG_SOURCE_INPUT_DEVICE,
    TAG_SOURCE_INPUT_MODEL,
    TAG_SOURCE_INPUT_SERIAL,
    TAG_SOURCE_INPUT_PITCH,
    TAG_SOURCE_GAMMA,
    TAG_FILM_FORMAT,
    TAG_FILM_FRAME,
    TAG_FILM_FRAME_RATE,
    TAG_FILM_FRAME_ID,
    TAG_FILM_SLATE,

    _TAG_SIZE
};

//! Get the image tag labels.

const List<String> & label_tag();

//! Create film print LUT.

void film_print(Pixel::F32_T *, int size, const Film_Print &);

//! Create inverse film print LUT.

void film_unprint(Pixel::F32_T *, int size, const Film_Unprint &);

//! Create film print LUT.

Pixel_Data film_print_lut(const Film_Print &);

//! Create inverse film print LUT.

Pixel_Data film_unprint_lut(const Film_Unprint &);

bool operator == (const Film_Print &, const Film_Print &);
bool operator == (const Film_Unprint &, const Film_Unprint &);

String & operator >> (String &, Film_Print &) throw (String);
String & operator >> (String &, Film_Unprint &) throw (String);
String & operator >> (String &, COLOR_PROFILE &) throw (String);
String & operator >> (String &, CONVERT &) throw (String);

String & operator << (String &, const Film_Print &);
String & operator << (String &, const Film_Unprint &);
String & operator << (String &, COLOR_PROFILE);
String & operator << (String &, CONVERT);

} // djv_cineon

#endif // DJV_CINEON_H

