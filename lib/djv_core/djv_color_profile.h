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

//! \file djv_color_profile.h

#ifndef DJV_COLOR_PROFILE_H
#define DJV_COLOR_PROFILE_H

#include <djv_pixel_data.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \struct Color_Profile
//!
//! This class provides a color profile.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Color_Profile
{
    //! This struct provides exposure values.

    struct DJV_CORE_EXPORT Exposure
    {
        //! Constructor.
        
        Exposure();

        double value;
        double defog;
        double knee_low;
        double knee_high;
    };

    //! Color profile.

    enum PROFILE
    {
        RAW,
        GAMMA,
        LUT,
        EXPOSURE,

        _PROFILE_SIZE
    };

    //! Get the color profile labels.

    static const List<String> & label_profile();

    //! Constructor.

    Color_Profile();

    PROFILE    type;
    double     gamma;
    Pixel_Data lut;
    Exposure   exposure;
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT bool operator == (
    const Color_Profile::Exposure &,
    const Color_Profile::Exposure &);
DJV_CORE_EXPORT bool operator == (
    const Color_Profile &,
    const Color_Profile &);

DJV_CORE_EXPORT bool operator != (
    const Color_Profile::Exposure &,
    const Color_Profile::Exposure &);
DJV_CORE_EXPORT bool operator != (
    const Color_Profile &,
    const Color_Profile &);

DJV_CORE_EXPORT String & operator >> (String &, Color_Profile::Exposure &)
    throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Color_Profile::PROFILE &)
    throw (String);

DJV_CORE_EXPORT String & operator << (
    String &,
    const Color_Profile::Exposure &);
DJV_CORE_EXPORT String & operator << (String &, Color_Profile::PROFILE);

DJV_CORE_EXPORT Debug & operator << (Debug &, Color_Profile::PROFILE);
DJV_CORE_EXPORT Debug & operator << (Debug &, const Color_Profile &);

} // djv

#endif // DJV_COLOR_PROFILE_H

