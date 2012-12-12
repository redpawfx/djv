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

//! \file djv_debug.cpp

#include <djv_debug.h>

#include <djv_string.h>

#include <stdio.h>
#include <stdlib.h>

namespace djv
{

//------------------------------------------------------------------------------
// Debug
//------------------------------------------------------------------------------

namespace
{

//! \todo Global variable.

int indent = 0;

} // namespace

void Debug::init(const String & in)
{
    *this << LINE_BEGIN << in << LINE_END;
    *this << LINE_BEGIN << "{" << LINE_END;

    indent += 4;
}

Debug::Debug(const String & in)
{
    init(in);
}

Debug::Debug(const String & prefix, const String & in)
{
    init(prefix + ": " + in);
}

Debug::~Debug()
{
    _timer.check();

    indent -= 4;

    *this <<
        LINE_BEGIN <<
        "} (" <<
        _timer.seconds() <<
        " seconds, " <<
        _timer.fps() <<
        " fps)" <<
        LINE_END;
}

Debug & Debug::operator << (LINE in)
{
    switch (in)
    {
        case LINE_BEGIN:
            _tmp = "debug " + String(' ', indent);
            break;

        case LINE_END:
            ::printf("%s\n", _tmp.c_str());
            ::fflush(stdout);
            break;
    }

    return *this;
}

void Debug::add(const String & in)
{
    _tmp += in;
}

namespace
{

String bits(uint32_t in, int size)
{
    String out;

    for (int i = 0; i < size; ++i)
    {
        out += ((in >> i) & 1) ? '1' : '0';
    }

    return out;
}

} // namespace

String Debug::bits_u8(uint8_t  in)
{
    return bits(in, 8);
}

String Debug::bits_u16(uint16_t in)
{
    return bits(in, 16);
}

String Debug::bits_u32(uint32_t in)
{
    return bits(in, 32);
}

//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, const String & in)
{
    debug.add(in);

    return debug;
}

Debug & operator << (Debug & debug, const char * in)
{
    debug.add(in);

    return debug;
}

Debug & operator << (Debug & debug, bool in)
{
    static const List<String> label = String_Util::label_bool();

    return debug << label[in];
}

Debug & operator << (Debug & debug, int in)
{
    return debug << String_Util::int_to_string(in);
}

Debug & operator << (Debug & debug, int64_t in)
{
    return debug << String_Util::int_to_string(in);
}

Debug & operator << (Debug & debug, uint in)
{
    return debug << String_Util::uint_to_string(in);
}

Debug & operator << (Debug & debug, uint64_t in)
{
    return debug << String_Util::uint_to_string(in);
}

Debug & operator << (Debug & debug, double in)
{
    return debug << String_Util::float_to_string(in);
}

/*Debug & operator << (Debug & debug, size_t in)
{
  return debug << String_Util::uint_to_string(in);
}*/

Debug & operator << (Debug & debug, const List<String> & in)
{
    return debug << String_Util::join(in, " ");
}

Debug & operator << (Debug & debug, const Set<String> & in)
{
    return debug << String_Util::join(in, " ");
}

} // djv

