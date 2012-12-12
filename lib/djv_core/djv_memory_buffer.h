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

//! \file djv_memory_buffer.h

#ifndef DJV_MEMORY_BUFFER_H
#define DJV_MEMORY_BUFFER_H

#include <djv_type.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Memory_Buffer
//!
//! This class provides a memory buffer.
//------------------------------------------------------------------------------

template<typename T>
class Memory_Buffer
{
public:

    //! Constructor.

    inline Memory_Buffer();

    //! Constructor.

    inline Memory_Buffer(const Memory_Buffer &);

    //! Constructor.

    inline Memory_Buffer(size_t);

    //! Destructor.

    inline ~Memory_Buffer();

    inline Memory_Buffer & operator = (const Memory_Buffer &);

    //! Set the size.

    inline void size(size_t);

    //!  Get the size.

    inline size_t size() const;

    //! Get a pointer to the memory.

    inline T * data();

    //! Get a pointer to the memory.

    inline const T * data() const;

    //! Zero the memory.

    inline void zero();

    inline T * operator () ();

    inline const T * operator () () const;

private:

    inline void del();

    T * _data;
    size_t _size;
};

} // djv

#include <djv_memory_buffer_inline.h>

#endif // DJV_MEMORY_BUFFER_H

