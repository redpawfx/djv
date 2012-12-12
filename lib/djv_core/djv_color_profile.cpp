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

//! \file djv_color_profile.cpp

#include <djv_color_profile.h>

#include <djv_assert.h>

namespace djv
{

//------------------------------------------------------------------------------
// Color_Profile::Exposure
//------------------------------------------------------------------------------

Color_Profile::Exposure::Exposure() :
    value    (0.0),
    defog    (0.0),
    knee_low (0.0),
    knee_high(5.0)
{}

//------------------------------------------------------------------------------
// Color_Profile
//------------------------------------------------------------------------------

Color_Profile::Color_Profile() :
    type (RAW),
    gamma(2.2)
{}

const List<String> & Color_Profile::label_profile()
{
    static const List<String> data = List<String>() <<
        "Raw" <<
        "Gamma" <<
        "LUT" <<
        "Exposure";

    DJV_ASSERT(data.size() == _PROFILE_SIZE);

    return data;
}

//------------------------------------------------------------------------------

bool operator == (
    const Color_Profile::Exposure & a,
    const Color_Profile::Exposure & b)
{
    return
        a.value     == b.value     &&
        a.defog     == b.defog     &&
        a.knee_low  == b.knee_low  &&
        a.knee_high == b.knee_high;
}

bool operator != (
    const Color_Profile::Exposure & a,
    Color_Profile::Exposure & b)
{
    return ! (a == b);
}

bool operator == (const Color_Profile & a, const Color_Profile & b)
{
    return
        a.type     == b.type     &&
        a.gamma    == b.gamma    &&
        a.lut      == b.lut      &&
        a.exposure == b.exposure;
}

bool operator != (const Color_Profile & a, const Color_Profile & b)
{
    return ! (a == b);
}

String & operator >> (String & in, Color_Profile::Exposure & out) throw (String)
{
    return in >> out.value >> out.defog >> out.knee_low >> out.knee_high;
}

String & operator << (String & out, const Color_Profile::Exposure & in)
{
    return out << in.value << in.defog << in.knee_low << in.knee_high;
}

_DJV_STRING_OPERATOR_LABEL(
    Color_Profile::PROFILE,
    Color_Profile::label_profile())

Debug & operator << (Debug & debug, Color_Profile::PROFILE in)
{
    return debug << String_Util::label(in);
}

Debug & operator << (Debug & debug, const Color_Profile & in)
{
    return debug <<
        in.type <<
        ", gamma: "    << in.gamma    <<
        ", lut: "      << in.lut      <<
        ", exposure: " << in.exposure;
}

} // djv
