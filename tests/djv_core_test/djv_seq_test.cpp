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

//! \file djv_seq_test.cpp

#include <djv_assert.h>
#include <djv_debug.h>
#include <djv_seq.h>

using namespace djv;

void seq_to_string()
{
    DJV_DEBUG("seq_to_string");

    const struct Data
    {
        Seq a;
        String b;
    } data [] =
    {
        { Seq(List<int64_t>()), "" },
        { Seq(List<int64_t>() << 1), "1" },
        { Seq(List<int64_t>() << 1 << 2), "1-2" },
        { Seq(List<int64_t>() << 1 << 2 << 3), "1-3" },
        { Seq(List<int64_t>() << 1 << 2 << 3, 4), "0001-0003" },
        { Seq(List<int64_t>() << 3 << 2 << 1), "3-1" },
        { Seq(List<int64_t>() << 1 << 2 << 3 << 5), "1-3,5" },
        { Seq(List<int64_t>() << 1 << 2 << 3 << 5 << 6), "1-3,5-6" },
        { Seq(List<int64_t>() << 1 << 2 << 3 << 5 << 6 << 7), "1-3,5-7" },
        { Seq(List<int64_t>() << 1 << 2 << 3 << 3 << 2 << 1), "1-3,3-1" }
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        const String tmp = Seq::seq_to_string(data[i].a);

        DJV_DEBUG_PRINT(data[i].a.list << " = " << tmp);

        DJV_ASSERT(tmp == data[i].b);
    }
}

void string_to_seq()
{
    DJV_DEBUG("string_to_seq");

    const struct Data
    {
        String a;
        Seq b;
    } data [] =
    {
        { "", Seq(List<int64_t>() << 0) },
        { "1", Seq(List<int64_t>() << 1) },
        { "1-2", Seq(List<int64_t>() << 1 << 2) },
        { "1-3", Seq(List<int64_t>() << 1 << 2 << 3) },
        { "0001-0003", Seq(List<int64_t>() << 1 << 2 << 3, 4) },
        { "3-1", Seq(List<int64_t>() << 3 << 2 << 1) },
        { "1,2,3", Seq(List<int64_t>() << 1 << 2 << 3) },
        { "1-3,5", Seq(List<int64_t>() << 1 << 2 << 3 << 5) },
        { "1-3,5-6", Seq(List<int64_t>() << 1 << 2 << 3 << 5 << 6) },
        { "1-3,5-7", Seq(List<int64_t>() << 1 << 2 << 3 << 5 << 6 << 7) },
        { "1-3,3-1", Seq(List<int64_t>() << 1 << 2 << 3 << 3 << 2 << 1) }
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        const Seq tmp = Seq::string_to_seq(data[i].a);

        DJV_DEBUG_PRINT(data[i].a << " = " << tmp);

        DJV_ASSERT(tmp == data[i].b);
    }
}

int main(int argc, char ** argv)
{
    seq_to_string();
    string_to_seq();

    return 0;
}

