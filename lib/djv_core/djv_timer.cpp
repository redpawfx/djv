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

//! \file djv_timer.cpp

#include <djv_timer.h>

#if defined(DJV_WINDOWS)
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif
#include <float.h>
#include <time.h>

namespace djv
{

//------------------------------------------------------------------------------
// Timer
//------------------------------------------------------------------------------

Timer::Timer()
{
#if defined(DJV_WINDOWS)

    //_t0 = _t1 = 0;
    _t0.QuadPart = 0;
    _t1.QuadPart = 0;
    ::QueryPerformanceFrequency(&_frequency);

#else // DJV_WINDOWS

    _t0.tv_sec = _t0.tv_usec = 0;
    _t1.tv_sec = _t1.tv_usec = 0;

#endif // DJV_WINDOWS

    start();
}

void Timer::start()
{
#if defined(DJV_WINDOWS)

    //_t0 = ::GetTickCount();
    ::QueryPerformanceCounter(&_t0);

#else // DJV_WINDOWS

    ::gettimeofday(&_t0, 0);

#endif // DJV_WINDOWS

    _t1 = _t0;
}

void Timer::check()
{
#if defined(DJV_WINDOWS)

    //_t1 = ::GetTickCount();
    ::QueryPerformanceCounter(&_t1);

#else // DJV_WINDOWS

    ::gettimeofday(&_t1, 0);

#endif // DJV_WINDOWS
}

#if ! defined(DJV_WINDOWS)

namespace
{

inline void sub(const timeval & a, const timeval & b, timeval * out)
{
    out->tv_sec = a.tv_sec - b.tv_sec;
    out->tv_usec = a.tv_usec - b.tv_usec;

    if (out->tv_usec < 0)
    {
        --out->tv_sec;
        out->tv_usec += 1000000;
    }
}

} // namespace

#endif // ! DJV_WINDOWS

double Timer::seconds() const
{
    double out = 0.0;

#if defined(DJV_WINDOWS)

    //out = (_t1 - _t0) / 1000.0;
    if (_frequency.QuadPart)
        out = (_t1.QuadPart - _t0.QuadPart) /
              static_cast<double>(_frequency.QuadPart);

#else // DJV_WINDOWS

    timeval t;
    sub(_t1, _t0, &t);
    out = t.tv_sec + t.tv_usec / 1000000.0;

#endif // DJV_WINDOWS

    return out;
}

double Timer::fps() const
{
    const double seconds = this->seconds();
    return (seconds != 0.0) ? (1.0 / seconds) : 0.0;
}

} // djv

