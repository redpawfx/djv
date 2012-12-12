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

//! \file djv_math.cpp

#include <djv_math.h>

#if defined(DJV_WINDOWS)
#include <windows.h>
#else
#include <sys/time.h>
#endif
#include <math.h>
#include <stdlib.h>

namespace djv
{

//------------------------------------------------------------------------------
// Math
//------------------------------------------------------------------------------

const double Math::pi    = 3.1415927;
const double Math::pi_x2 = 6.2831853;
const double Math::pi_2  = 1.5707963;
const double Math::pi_4  = 0.7853981;

int Math::pow(int in, int power)
{
    return floor(::pow(static_cast<double>(in), static_cast<double>(power)));
}

double Math::pow(double in, double power)
{
    return ::pow(in, power);
}

double Math::sqrt(double in)
{
    return ::sqrt(in);
}

double Math::exp(double in)
{
    return ::exp(in);
}

double Math::log(double in)
{
    return ::log(in);
}

double Math::log_10(double in)
{
    return ::log10(in);
}

double Math::anti_log_10(double in)
{
    return exp(in * log(10.0));
}

double Math::bias(double in, double bias)
{
    return pow(in, log(bias) / log(0.5));
}

//! \todo Is this still necessary?

namespace
{

const double almost_zero = FLT_MIN * 2.0;
const double almost_one = 1.0 - almost_zero;

} // namespace

double Math::gain(double in, double gain)
{
    if (in < almost_zero)
    {
        return 0.0;
    }
    else if (in > almost_one)
    {
        return 1.0;
    }

    const double tmp = log(1.0 - gain) / log(0.5);

    if (in < 0.5)
    {
        return pow(2.0 * in, tmp) / 2.0;
    }
    else
    {
        return 1.0 - pow(2.0 * (1.0 - in), tmp) / 2.0;
    }
}

double Math::soft_clip(double in, double value)
{
    const double tmp = 1.0 - value;

    if (in > tmp)
    {
        in = tmp + (1.0 - exp(-(in - tmp) / value)) * value;
    }

    return in;
}

double Math::sin(double in)
{
    return ::sin(in);
}
double Math::cos(double in)
{
    return ::cos(in);
}
double Math::tan(double in)
{
    return ::tan(in);
}

double Math::arc_sin(double in)
{
    return ::asin(in);
}
double Math::arc_cos(double in)
{
    return ::acos(in);
}
double Math::arc_tan(double in)
{
    return ::atan(in);
}
double Math::arc_tan(double a, double b)
{
    return ::atan2(a, b);
}

//! \todo Zero?

double Math::sec(double in)
{
    const double tmp = cos(in);
    return tmp != 0.0 ? (1.0 / tmp) : 0.0;
}

double Math::co_sec(double in)
{
    const double tmp = sin(in);
    return tmp != 0.0 ? (1.0 / tmp) : 0.0;
}

double Math::co_tan(double in)
{
    const double tmp = tan(in);
    return tmp != 0.0 ? (1.0 / tmp) : 0.0;
}

int Math::ceil(double in)
{
    return static_cast<int>(::ceil(in));
}

int Math::floor(double in)
{
    return static_cast<int>(::floor(in));
}

int Math::round(double in)
{
    //return static_cast<int>(::round(in));

    //! \todo No Windows version of round?

    return static_cast<int>(in + 0.5);
}

namespace
{

double _rand()
{
    return ::rand() / static_cast<double>(RAND_MAX);
}

} // namespace

double Math::rand()
{
    return _rand();
}

double Math::rand(double in)
{
    return in * _rand();
}

double Math::rand(double min, double max)
{
    return min + (max - min) * _rand();
}

void Math::rand_seed(uint in)
{
    if (! in)
    {
        //! \todo Seed with current time.

#if defined(DJV_WINDOWS)

        in = ::GetTickCount();

#else // DJV_WINDOWS

        struct timeval tmp = { 0, 0 };
        gettimeofday(&tmp, 0);
        in = tmp.tv_usec;

#endif // DJV_WINDOWS
    }

    srand(in);
}

} // djv
