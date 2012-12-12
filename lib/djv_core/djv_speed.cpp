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

//! \file djv_speed.cpp

#include <djv_speed.h>

#include <djv_assert.h>
#include <djv_math.h>

namespace djv
{

//------------------------------------------------------------------------------
// Speed
//------------------------------------------------------------------------------

const List<String> & Speed::label_fps()
{
    static const List<String> data = List<String>() <<
        "1" <<
        "3" <<
        "6" <<
        "12" <<
        "15" <<
        "16" <<
        "18" <<
        "23.98" <<
        "24" <<
        "25" <<
        "29.97" <<
        "30" <<
        "50" <<
        "59.94" <<
        "60" <<
        "120";

    DJV_ASSERT(data.size() == _FPS_SIZE);

    return data;
}

Speed::FPS Speed::default_fps = Speed::FPS_24;

Speed::Speed()
{
    set(default_fps);
}

Speed::Speed(int scale, int duration) :
    scale(scale),
    duration(duration)
{}

Speed::Speed(FPS in)
{
    set(in);
}

void Speed::set(FPS in)
{
    switch (in)
    {
        case FPS_1:
            scale = 1;
            duration = 1;
            break;

        case FPS_3:
            scale = 3;
            duration = 1;
            break;

        case FPS_6:
            scale = 6;
            duration = 1;
            break;

        case FPS_12:
            scale = 12;
            duration = 1;
            break;

        case FPS_15:
            scale = 15;
            duration = 1;
            break;

        case FPS_16:
            scale = 16;
            duration = 1;
            break;

        case FPS_18:
            scale = 18;
            duration = 1;
            break;

        case FPS_23_98:
            scale = 24000;
            duration = 1001;
            break;

        case FPS_24:
            scale = 24;
            duration = 1;
            break;

        case FPS_25:
            scale = 25;
            duration = 1;
            break;

        case FPS_29_97:
            scale = 30000;
            duration = 1001;
            break;

        case FPS_30:
            scale = 30;
            duration = 1;
            break;

        case FPS_50:
            scale = 50;
            duration = 1;
            break;

        case FPS_59_94:
            scale = 60000;
            duration = 1001;
            break;

        case FPS_60:
            scale = 60;
            duration = 1;
            break;

        case FPS_120:
            scale = 120;
            duration = 1;
            break;

        default:
            break;
    }
}

bool Speed::is_valid() const
{
    return scale != 0 && duration != 0;
}

double Speed::speed_to_float(const Speed & in)
{
    return in.scale / static_cast<double>(in.duration);
}

Speed Speed::float_to_speed(double in)
{
    //! \todo Is this still necessary?

    if (Math::abs(in - speed_to_float(FPS_1)) < 0.001)
    {
        return FPS_1;
    }
    else if (Math::abs(in - speed_to_float(FPS_3)) < 0.001)
    {
        return FPS_3;
    }
    else if (Math::abs(in - speed_to_float(FPS_6)) < 0.001)
    {
        return FPS_6;
    }
    else if (Math::abs(in - speed_to_float(FPS_12)) < 0.001)
    {
        return FPS_12;
    }
    else if (Math::abs(in - speed_to_float(FPS_15)) < 0.001)
    {
        return FPS_15;
    }
    else if (Math::abs(in - speed_to_float(FPS_16)) < 0.001)
    {
        return FPS_16;
    }
    else if (Math::abs(in - speed_to_float(FPS_18)) < 0.001)
    {
        return FPS_18;
    }
    else if (Math::abs(in - speed_to_float(FPS_23_98)) < 0.001)
    {
        return FPS_23_98;
    }
    else if (Math::abs(in - speed_to_float(FPS_24)) < 0.001)
    {
        return FPS_24;
    }
    else if (Math::abs(in - speed_to_float(FPS_25)) < 0.001)
    {
        return FPS_25;
    }
    else if (Math::abs(in - speed_to_float(FPS_29_97)) < 0.001)
    {
        return FPS_29_97;
    }
    else if (Math::abs(in - speed_to_float(FPS_30)) < 0.001)
    {
        return FPS_30;
    }
    else if (Math::abs(in - speed_to_float(FPS_50)) < 0.001)
    {
        return FPS_50;
    }
    else if (Math::abs(in - speed_to_float(FPS_59_94)) < 0.001)
    {
        return FPS_59_94;
    }
    else if (Math::abs(in - speed_to_float(FPS_60)) < 0.001)
    {
        return FPS_60;
    }
    else if (Math::abs(in - speed_to_float(FPS_120)) < 0.001)
    {
        return FPS_120;
    }

    return Speed(Math::round(in));
}

//------------------------------------------------------------------------------

bool operator == (const Speed & a, const Speed & b)
{
    return a.scale == b.scale && a.duration == b.duration;
}

bool operator != (const Speed & a, const Speed & b)
{
    return ! (a == b);
}

String & operator >> (String & in, Speed & out) throw (String)
{
    return in >> out.scale >> out.duration;
}

String & operator << (String & out, const Speed & in)
{
    return out << in.scale << in.duration;
}

_DJV_STRING_OPERATOR_LABEL(Speed::FPS, Speed::label_fps())

Debug & operator << (Debug & debug, const Speed & in)
{
    return debug << Speed::speed_to_float(in);
}

} // djv

