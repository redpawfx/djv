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

//! \file djv_sgi.h

#ifndef DJV_SGI_H
#define DJV_SGI_H

#include <djv_file_io.h>
#include <djv_image_io.h>

//! \namespace djv_sgi
//!
//! This plugin provides support for the Silicon Graphics image file format.
//!
//! Supports:
//!
//! - Images: 8-bit, 16-bit, Luminance, Luminance Alpha, RGB, RGBA
//! - File compression
//!
//! References:
//!
//! - Paul Haeberli, "The SGI Image File Format, Version 1.00"

namespace djv_sgi
{
using namespace djv;

static const String name = "SGI";

static const List<String> extensions = List<String>() <<
    ".sgi"  <<
    ".rgba" <<
    ".rgb"  <<
    ".bw";

//! Compression.

enum COMPRESSION
{
    COMPRESSION_NONE,
    COMPRESSION_RLE,

    _COMPRESSION_SIZE
};

//! Get compression labels.

const List<String> & label_compression();

//! Load information.

void info_load(
    File_Io &       io,
    Image_Io_Info & info,
    bool *          compression) throw (Error);

//! Save information.

void info_save(
    File_Io &             io,
    const Image_Io_Info & info,
    bool                  compression) throw (Error);

//! Load RLE data.

bool rle_load(
    const void * in,
    const void * end,
    void *       out,
    int          size,
    int          bytes,
    bool         endian);

//! Save RLE data.

size_t rle_save(
    const void * in,
    void *       out,
    int          size,
    int          bytes,
    bool         endian);

String & operator >> (String &, COMPRESSION &) throw (String);

String & operator << (String &, COMPRESSION);

} // djv_sgi

#endif // DJV_SGI_H

