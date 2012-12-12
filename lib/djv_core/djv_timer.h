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

//! \file djv_timer.h

#ifndef DJV_TIMER_H
#define DJV_TIMER_H

#include <djv_type.h>
#include <djv_string.h>

//#if ! defined(DJV_WINDOWS)
#include <sys/time.h>
//#endif

namespace djv
{

//------------------------------------------------------------------------------
//! \class Timer
//!
//! This class provides a timer.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Timer
{
public:

    //! Constructor.

    Timer();

    //! Start the timer.

    void start();

    //! Check the timer.

    void check();

    //! Get elapsed time in seconds. You should call check() before calling this
    //! function.

    double seconds() const;

    //! Get elapsed time in frames per second. You should call check() before
    //! calling this function.

    double fps() const;

private:

#if defined(DJV_WINDOWS)
    //DWORD _t0;
    //DWORD _t1;
    LARGE_INTEGER _t0;
    LARGE_INTEGER _t1;
    LARGE_INTEGER _frequency;
#else
    ::timeval _t0;
    ::timeval _t1;
#endif
};

} // djv

#endif // DJV_TIMER_H

