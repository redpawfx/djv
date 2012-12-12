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

//! \file djv_debug.h

#ifndef DJV_DEBUG_H
#define DJV_DEBUG_H

#include <djv_list.h>
#include <djv_set.h>
#include <djv_string.h>
#include <djv_timer.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Debug
//!
//! This class provides debugging messages.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Debug
{
public:

    //! Constructor.

    Debug(const String &);

    //! Constructor.

    Debug(const String & prefix, const String &);

    //! Destructor.

    ~Debug();

    //! Add a message.

    void add(const String &);

    //! Line beginning and ending.

    enum LINE
    {
        LINE_BEGIN,
        LINE_END
    };

    Debug & operator << (LINE);

    //! Convert bits to a string.

    static String bits_u8(uint8_t);

    //! Convert bits to a string.

    static String bits_u16(uint16_t);

    //! Convert bits to a string.

    static String bits_u32(uint32_t);

private:

    void init(const String &);

    String _tmp;
    Timer  _timer;
};

//! Debugging message macros.

#define DJV_DEBUG(in) \
  \
  djv::Debug _debug(__FILE__, in)

#define DJV_DEBUG_PRINT(in) \
  \
  _debug << djv::Debug::LINE_BEGIN << in << djv::Debug::LINE_END

//------------------------------------------------------------------------------

DJV_CORE_EXPORT Debug & operator << (Debug &, const String &);
DJV_CORE_EXPORT Debug & operator << (Debug &, const char *);
DJV_CORE_EXPORT Debug & operator << (Debug &, bool);
DJV_CORE_EXPORT Debug & operator << (Debug &, int);
DJV_CORE_EXPORT Debug & operator << (Debug &, int64_t);
DJV_CORE_EXPORT Debug & operator << (Debug &, uint);
DJV_CORE_EXPORT Debug & operator << (Debug &, uint64_t);
DJV_CORE_EXPORT Debug & operator << (Debug &, double);
//DJV_CORE_EXPORT Debug & operator << (Debug &, size_t);
DJV_CORE_EXPORT Debug & operator << (Debug &, const List<String> &);
DJV_CORE_EXPORT Debug & operator << (Debug &, const Set<String> &);
template<class T>
inline Debug & operator << (Debug &, const List<T> &);
template<class T>
inline Debug & operator << (Debug &, const Set<T> &);

} // djv

#include <djv_debug_inline.h>

#endif // DJV_DEBUG_H

