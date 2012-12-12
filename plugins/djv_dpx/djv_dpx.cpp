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

//! \file djv_dpx.cpp

#include <djv_dpx.h>

#include <djv_assert.h>

namespace djv_dpx
{

const List<String> & label_version()
{
    static const List<String> data = List<String>() <<
        "1.0" <<
        "2.0";

    DJV_ASSERT(data.size() == _VERSION_SIZE);

    return data;
}

const List<String> & label_type()
{
    static const List<String> data = List<String>() <<
        "Auto" <<
        "U10";

    DJV_ASSERT(data.size() == _TYPE_SIZE);

    return data;
}

const List<String> & label_endian()
{
    static const List<String> data = List<String>() <<
        "Auto" <<
        Memory::label_endian();

    DJV_ASSERT(data.size() == _ENDIAN_SIZE);

    return data;
}

const List<String> & label_tag()
{
    static const List<String> data = List<String>() <<
        "Source Offset" <<
        "Source Center" <<
        "Source Size" <<
        "Source File" <<
        "Source Time" <<
        "Source Input Device" <<
        "Source Input Serial" <<
        "Source Border" <<
        "Source Pixel Aspect" <<
        "Source Scan Size" <<
        "Film Format" <<
        "Film Frame" <<
        "Film Sequence" <<
        "Film Hold" <<
        "Film Frame Rate" <<
        "Film Shutter" <<
        "Film Frame ID" <<
        "Film Slate" <<
        "TV Interlace" <<
        "TV Field" <<
        "TV Video Signal" <<
        "TV Sample Rate" <<
        "TV Frame Rate" <<
        "TV Time Offset" <<
        "TV Gamma" <<
        "TV Black Level" <<
        "TV Black Gain" <<
        "TV Break Point" <<
        "TV White Level" <<
        "TV Integration Times";

    DJV_ASSERT(data.size() == _TAG_SIZE);

    return data;
}

_DJV_STRING_OPERATOR_LABEL(VERSION, label_version())
_DJV_STRING_OPERATOR_LABEL(TYPE, label_type())
_DJV_STRING_OPERATOR_LABEL(ENDIAN, label_endian())

} // djv_dpx

