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

//! \file djv_tiff.h

#ifndef DJV_TIFF_H
#define DJV_TIFF_H

#include <djv_string.h>

#include <tiffio.h>

//! \namespace djv_tiff
//!
//! This plugin provides support for the Tagged Image File Format (TIFF).
//!
//! Requires:
//!
//! - libtiff - http://www.libtiff.org
//!
//! Supports:
//!
//! - Images: 8-bit, 16-bit, 32-bit float, Luminance, Luminance Alpha, RGB,
//!   RGBA
//! - Interleaved channels only
//! - File compression

namespace djv_tiff
{
using namespace djv;

static const String name = "TIFF";

static const List<String> extensions = List<String>() <<
    ".tiff" << ".TIFF" <<
    ".tif"  << ".TIF";

//! Compression.

enum COMPRESSION
{
    //! \todo libtiff defines

    _COMPRESSION_NONE,
    _COMPRESSION_RLE,
    _COMPRESSION_LZW,

    _COMPRESSION_SIZE
};

//! Get compression labels.

const List<String> & label_compression();

//! Initialize libtiff.

void tiff_init();

//! Load a palette.

void palette_load(
    uint8_t *  out,
    int        size,
    int        bytes,
    uint16_t * red,
    uint16_t * green,
    uint16_t * blue);

String & operator >> (String &, COMPRESSION &) throw (String);

String & operator << (String &, COMPRESSION);

} // djv_tiff

#endif // DJV_TIFF_H

