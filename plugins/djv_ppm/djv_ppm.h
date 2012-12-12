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

//! \file djv_ppm.h

#ifndef DJV_PPM_H
#define DJV_PPM_H

#include <djv_file_io.h>
#include <djv_image_io.h>

//! \namespace djv_ppm
//!
//! This plugin supports the NetPBM image file formats.
//!
//! Supports:
//!
//! - Images: 1-bit, 8-bit, 16-bit, Luminance, RGB
//! - Binary and ASCII data
//!
//! References:
//!
//! - Netpbm, "PPM Format Specification"
//!   http://netpbm.sourceforge.net/doc/ppm.html

namespace djv_ppm
{
using namespace djv;

static const String name = "PPM";

static const List<String> extensions = List<String>() <<
    ".ppm" <<
    ".pnm" <<
    ".pgm" <<
    ".pbm";

//! Type.

enum TYPE
{
    TYPE_AUTO,
    TYPE_U1,

    _TYPE_SIZE
};

//! Get the type labels.

const List<String> & label_type();

//! Data.

enum DATA
{
    DATA_ASCII,
    DATA_BINARY,

    _DATA_SIZE
};

//! Get the data labels.

const List<String> & label_data();

//! Get the number of bytes in a scanline.

size_t bytes_scanline(
    int  width,
    int  channels,
    int  bit_depth,
    DATA data);

//! Load ASCII data.

void ascii_load(
    File_Io & io,
    void *    out,
    int       size,
    int       bit_depth) throw (Error);

//! Save ASCII data.

size_t ascii_save(
    const void * in,
    void *       out,
    int          size,
    int          bit_depth);

String & operator >> (String &, TYPE &) throw (String);
String & operator >> (String &, DATA &) throw (String);

String & operator << (String &, TYPE);
String & operator << (String &, DATA);

} // djv_ppm

#endif // DJV_PPM_H

