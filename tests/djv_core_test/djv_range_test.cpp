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

//! \file djv_range_test.cpp

#include <djv_assert.h>
#include <djv_debug.h>
#include <djv_range.h>

using namespace djv;

int main(int argc, char ** argv)
{
    typedef Range<int64_t> Range;

    const struct Data
    {
        List<int64_t> a;
        List<Range> b;
    }
    data [] =
    {
        {
            List<int64_t>() << 1,
            List<Range>() << Range(1, 1)
        },
        {
            List<int64_t>() << 1 << 2,
            List<Range>() << Range(1, 2)
        },
        {
            List<int64_t>() << 1 << 2 << 3,
            List<Range>() << Range(1, 3)
        },
        {
            List<int64_t>() << 1 << 3,
            List<Range>() << Range(1) << Range(3)
        },
        {
            List<int64_t>() << 1 << 2 << 4,
            List<Range>() << Range(1, 2) << Range(4)
        },
        {
            List<int64_t>() << 1 << 2 << 4 << 5,
            List<Range>() << Range(1, 2) << Range(4, 5)
        }
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        const List<Range> tmp = Range_Util::range(data[i].a);
        DJV_ASSERT(tmp == data[i].b);
    }

    return 0;
}
