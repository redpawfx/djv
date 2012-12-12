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

//! \file djv_lut.h

#ifndef DJV_LUT_H
#define DJV_LUT_H

#include <djv_file_io.h>
#include <djv_image_io.h>

//! \namespace djv_lut
//!
//! This plugin supports two-dimensional lookup table file formats.
//!
//! Supports:
//!
//! - Formats: Inferno, Kodak
//! - Images: 8-bit, 16-bit, Luminance, Luminance Alpha, RGB, RGBA; 10-bit RGB

namespace djv_lut
{
using namespace djv;

static const String name = "LUT";

static const List<String> extensions = List<String>() <<
    ".lut" <<
    ".1dl";

//! File format.

enum FORMAT
{
    FORMAT_AUTO,
    FORMAT_INFERNO,
    FORMAT_KODAK,

    _FORMAT_SIZE
};

//! Get the file format labels.

const List<String> & label_format();

//! File type.

enum TYPE
{
    TYPE_AUTO,
    TYPE_U8,
    TYPE_U10,
    TYPE_U16,

    _TYPE_SIZE
};

//! Get the file type labels.

const List<String> & label_type();

//! Open an Inferno LUT.

void inferno_open(File_Io &, Pixel_Data_Info &, TYPE) throw (Error);

//! Load an Inferno LUT.

void inferno_load(File_Io &, Image *) throw (Error);

//! Open a Kodak LUT.

void kodak_open(File_Io &, Pixel_Data_Info &, TYPE) throw (Error);

//! Load a Kodak LUT.

void kodak_load(File_Io &, Image *) throw (Error);

//! Open an Inferno LUT.

void inferno_open(File_Io &, const Pixel_Data_Info &) throw (Error);

//! Save an Inferno LUT.

void inferno_save(File_Io & io, const Pixel_Data *) throw (Error);

//! Open a Kodak LUT.

void kodak_open(File_Io &, const Pixel_Data_Info &) throw (Error);

//! Save a Kodak LUT.

void kodak_save(File_Io &, const Pixel_Data *) throw (Error);

String & operator >> (String &, FORMAT &) throw (String);
String & operator >> (String &, TYPE &) throw (String);

String & operator << (String &, FORMAT);
String & operator << (String &, TYPE);

} // djv_lut

#endif // DJV_LUT_H

