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

//! \file djv_rla.h

#ifndef DJV_RLA_H
#define DJV_RLA_H

#include <djv_file_io.h>

//! \namespace djv_rla
//!
//! This plugin supports the Wavefront RLA image file format.
//!
//! Supports:
//!
//! - Read-only
//! - Images: 8-bit, 16-bit, 32-bit float, Luminance, Luminance Alpha, RGB,
//!   RGBA
//! - File compression
//!
//! References:
//!
//! - James D. Murray, William vanRyper, "Encyclopedia of Graphics File Formats,
//!   Second Edition"

namespace djv_rla
{
using namespace djv;

static const String name = "RLA";

static const List<String> extensions = List<String>() <<
    ".rla" <<
    ".rpf";

//! Load RLE data.

void rle_load(
    File_Io & io,
    uint8_t * out,
    int       size,
    int       channels,
    int       bytes) throw (Error);

//! Load floating point data.

void float_load(
    File_Io & io,
    uint8_t * out,
    int       size,
    int       channels) throw (Error);

//! Skip file data.

void skip(File_Io &) throw (Error);

} // djv_rla

#endif // DJV_RLA_H

