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

//! \file djv_error.cpp

#include <djv_error.h>

#include <djv_string.h>

#include <stdio.h>

namespace djv
{

//------------------------------------------------------------------------------
// Error
//------------------------------------------------------------------------------

Error::Error()
{}

Error::Error(const String & in) :
    _value(in)
{}

Error::Error(const String & prefix, const String & in) :
    _value(prefix + " - " + in)
{}

const String & Error::string() const
{
    return _value;
}

Error::operator String () const
{
    return _value;
}

//------------------------------------------------------------------------------
// Error_Util
//------------------------------------------------------------------------------

String Error_Util::format(const Error & in)
{
    return String_Format(error_format).arg(in);
}

void Error_Util::print(const Error & in)
{
    ::printf("%s\n", format(in).c_str());
}

String Error_Util::last_error()
{
    String out;

#if defined(DJV_WINDOWS)

    struct Buffer
    {
        Buffer() : p(0) {}
        ~Buffer()
        {
            LocalFree(p);
        }
        LPTSTR * p;
    } buffer;

    ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        0,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&buffer.p,
        0,
        0);

    out = String((LPCTSTR)buffer.p);

#endif // DJV_WINDOWS

    return out;
}

const String
Error_Util::error_format = "[ERROR] %%";

} // djv

