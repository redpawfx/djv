//------------------------------------------------------------------------------
// Copyright (c) 2008-2009 Mikael Sundell
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

//! \file djv_iff.h

#ifndef DJV_IFF_H
#define DJV_IFF_H

#include <djv_file_io.h>
#include <djv_image_io.h>

//! \namespace djv_image::io_iff
//!
//! This plugin provides support for the Generic Interchange File Format (IFF).
//!
//! Supports:
//!
//! - Images: 8-bit, 16-bit, Luminance, Luminance Alpha, RGB, RGBA
//! - File compression
//!
//! References:
//!
//! - Affine Toolkit (Thomas E. Burge), riff.h and riff.c
//!   http://affine.org
//! - Autodesk Maya documentation, "Overview of Maya IFF"
//!
//! Implementation:
//!
//! - Mikael Sundell, mikael.sundell@gmail.com

namespace djv_iff
{
using namespace djv;

static const String name = "IFF";

static const List<String> extensions = List<String>() <<
    ".iff" <<
    ".z";

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
    int *           tiles,
    bool *          compression) throw (Error);

//! Save information.

void info_save(
    File_Io &             io,
    const Image_Io_Info & info,
    bool                  compression) throw (Error);

//! Load RLE compressed data.

size_t rle_load(
    const uint8_t * in,
    uint8_t *       end,
    int             size);

//! Save RLE compressed data.

size_t rle_save (
    const uint8_t * in,
    uint8_t *       out,
    int             size);

//! Get alignment size.

uint32_t align_size(
    uint32_t size,
    uint32_t alignment);

//! Get tile size.

V2i tile_size(
    uint32_t width,
    uint32_t height);

//! Get tile width.

const int & tile_width();

//! Get tile height.

const int & tile_height();

String & operator >> (String &, COMPRESSION &) throw (String);

String & operator << (String &, COMPRESSION);

} // djv_iff

#endif // DJV_IFF_H
