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

//! \file djv_memory.cpp

#include <djv_memory.h>

#include <string.h>
#if ! (defined(DJV_FREEBSD) || defined(DJV_OSX))
#include <malloc.h>
#include <stdlib.h>
#endif

namespace djv
{

//------------------------------------------------------------------------------
// Memory
//------------------------------------------------------------------------------

const uint64_t Memory::kilobyte = 1024;
const uint64_t Memory::megabyte = kilobyte * 1024;
const uint64_t Memory::gigabyte = megabyte * 1024;
const uint64_t Memory::terabyte = gigabyte * 1024;

const size_t Memory::align = 4096;

void * Memory::get(size_t in)
{
    //! \todo Is this still necessary?

#if ! (defined(DJV_WINDOWS) || defined(DJV_FREEBSD) || defined(DJV_OSX))
    return ::memalign(Memory::align, in);
#else
    return ::malloc(in);
#endif
}

void Memory::del(void * in)
{
    ::free(in);
}

void Memory::copy(const void * in, void * out, size_t size)
{
    ::memcpy(out, in, size);
}

void Memory::zero(void * out, size_t size)
{
    ::memset(out, 0, size);
}

int Memory::compare(const void * a, const void * b, size_t size)
{
    return ::memcmp(a, b, size);
}

const List<String> & Memory::label_endian()
{
    static const List<String> data = List<String>() <<
        "MSB" <<
        "LSB";

    //! \todo Pre-processor clash with DJV_ASSERT? (Ubuntu 8.04)

    //DJV_ASSERT(data.size() == _ENDIAN_SIZE);

    return data;
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Memory::ENDIAN, Memory::label_endian())

} // djv

