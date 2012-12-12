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

//! \file djv_memory.h

#ifndef DJV_MEMORY_H
#define DJV_MEMORY_H

#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \struct Memory
//!
//! This struct provides memory utilities.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Memory
{
    static const uint64_t kilobyte; //!< Kilobyte.
    static const uint64_t megabyte; //!< Megabyte.
    static const uint64_t gigabyte; //!< Gigabyte.
    static const uint64_t terabyte; //!< Terabyte.

    static const size_t align; //!< Memory alignment.

    //! Machine endian

    enum ENDIAN
    {
        MSB,  //!< Most siginificant byte first
        LSB,  //!< Least significant byte first

        _ENDIAN_SIZE
    };

    //! Allocate memory.

    static void * get(size_t);

    //! De-allocate memory.

    static void del(void *);

    //! Copy memory.

    static void copy(const void *, void *, size_t size);

    //! Fill memory with a value.

    template<typename T>
    static inline void set(T, void *, size_t size);

    //! Fill memory with zeroes.

    static void zero(void *, size_t size);

    //! Get the current machine's endian.

    static inline ENDIAN endian();

    //! Get the opposite of the given endian.

    static inline ENDIAN endian_opposite(ENDIAN);

    //! Compare memory.

    static int compare(const void *, const void *, size_t size);

    //! Endian conversion.

    static inline void endian(
        void *,
        size_t size,
        size_t word_size);

    //! Endian conversion.

    static inline void endian(
        const void *,
        void *,
        size_t size,
        size_t word_size);

    //! Get a size label.

    static String label_size(uint64_t);

    //! Get endian labels.

    static const List<String> & label_endian();
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT
String & operator >> (String &, Memory::ENDIAN &) throw (String);

DJV_CORE_EXPORT String & operator << (String &, Memory::ENDIAN);

} // djv

#include <djv_memory_inline.h>

#endif // DJV_MEMORY_H

