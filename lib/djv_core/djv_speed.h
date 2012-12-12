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

//! \file djv_speed.h

#ifndef DJV_SPEED_H
#define DJV_SPEED_H

#include <djv_debug.h>
#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \struct Speed
//!
//! This struct provides speed.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Speed
{
    //! Frames per second.

    enum FPS
    {
        FPS_1,
        FPS_3,
        FPS_6,
        FPS_12,
        FPS_15,
        FPS_16,
        FPS_18,
        FPS_23_98,
        FPS_24,
        FPS_25,
        FPS_29_97,
        FPS_30,
        FPS_50,
        FPS_59_94,
        FPS_60,
        FPS_120,

        _FPS_SIZE
    };

    //! Get the frames per second labels.

    static const List<String> & label_fps();

    //! The default frames per second.

    static FPS default_fps;

    //! Constructor.

    Speed();

    //! Constructor.

    Speed(int scale, int duration = 1);

    //! Constructor.

    Speed(FPS);

    int scale;
    int duration;

    //! Set the frames per second.

    void set(FPS);

    //! Get whether the speed is valid.

    bool is_valid() const;

    //! Convert a speed to a floating point value.

    static double speed_to_float(const Speed &);

    //! Convert a floating point value to a speed.

    static Speed float_to_speed(double);
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT bool operator == (const Speed &, const Speed &);

DJV_CORE_EXPORT bool operator != (const Speed &, const Speed &);

DJV_CORE_EXPORT String & operator >> (String &, Speed &) throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Speed::FPS &)
    throw (String);

DJV_CORE_EXPORT String & operator << (String &, const Speed &);
DJV_CORE_EXPORT String & operator << (String &, Speed::FPS);

DJV_CORE_EXPORT Debug & operator << (Debug &, const Speed &);

} // djv

#endif // DJV_SPEED_H
