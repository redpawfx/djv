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

//! \file djv_seq.h

#ifndef DJV_SEQ_H
#define DJV_SEQ_H

#include <djv_speed.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Seq
//!
//! This class provides a sequence of frames.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Seq
{
    //! Constructor.

    Seq();

    //! Constructor.

    Seq(const List<int64_t> &, int pad = 0, const Speed & = Speed());

    //! Constructor.

    Seq(int64_t start, int64_t end, int pad = 0, const Speed & = Speed());

    //! The list of frame numbers.

    List<int64_t> list;

    //! The frame number padding; e.g., 0010.

    int pad;

    //! The speed.

    Speed speed;

    //! Set the list of frame numbers.

    void set(int64_t start, int64_t end);

    //! Get the start frame.

    inline int64_t start() const;

    //! Get the end frame.

    inline int64_t end() const;

    //! Sequence compression.

    enum COMPRESS
    {
        COMPRESS_OFF,     //!< No sequence compression
        COMPRESS_SPARSE,  //!< Sparse sequence compression, for example: 1-3,5
        COMPRESS_RANGE,   //!< Range sequence compression, for example: 1-5

        _COMPRESS_SIZE
    };

    //! \todo Sizes larger than seq_max are probably an error.

    static const int64_t seq_max;

    //! Sort frame numbers in a sequence.

    static void sort(Seq &);

    //! Find the closest frame in a sequence.

    static int64_t find_closest(int64_t, const List<int64_t> &);

    //! Convert a sequence to a string.

    static String seq_to_string(const Seq &);

    //! Convert a string to a sequence.

    static Seq string_to_seq(const String &);

    //! Convert a frame to a string.

    static String frame_to_string(int64_t, int pad = 0);

    //! Convert a string to a frame.

    static inline int64_t string_to_frame(const String &, int * pad = 0);

    //! Get the compression labels.

    static const List<String> & label_compress();
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT String & operator >> (String &, Seq &) throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Seq::COMPRESS &) throw (String);

DJV_CORE_EXPORT String & operator << (String &, const Seq &);
DJV_CORE_EXPORT String & operator << (String &, Seq::COMPRESS);

inline bool operator == (const Seq &, const Seq &);

inline bool operator != (const Seq &, const Seq &);

DJV_CORE_EXPORT Debug & operator << (Debug &, const Seq &);

} // djv

#include <djv_seq_inline.h>

#endif // DJV_SEQ_H

