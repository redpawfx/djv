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

//! \file djv_seq_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// Seq
//------------------------------------------------------------------------------

inline int64_t Seq::start() const
{
    return list.size() ? list[0] : 0;
}

inline int64_t Seq::end() const
{
    return list.size() ? list[list.size() - 1] : 0;
}

inline int64_t Seq::string_to_frame(const String & in, int * pad)
{
    const size_t size = in.size();

    if (size)
        if ('#' == in[0])
        {
            return -1;
        }

    if (pad)
    {
        *pad = 0;

        if (size > 1)
            if ('0' == in[0])
            {
                *pad = static_cast<int>(size);
            }
    }

    return String_Util::string_to_int64(in);
}

//------------------------------------------------------------------------------

inline bool operator == (const Seq & a, const Seq & b)
{
    return a.list == b.list && a.pad == b.pad;
}

inline bool operator != (const Seq & a, const Seq & b)
{
    return ! (a == b);
}

} // djv

