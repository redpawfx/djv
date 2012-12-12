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

//! \file djv_file_io_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// File_Io
//------------------------------------------------------------------------------

inline size_t File_Io::size() const
{
    return _size;
}

inline void File_Io::set_8(const int8_t & in) throw (Error)
{
    set_8(&in, 1);
}

inline void File_Io::set_u8(const uint8_t & in) throw (Error)
{
    set_u8(&in, 1);
}

inline void File_Io::set_16(const int16_t & in) throw (Error)
{
    set_16(&in, 1);
}

inline void File_Io::set_u16(const uint16_t & in) throw (Error)
{
    set_u16(&in, 1);
}

inline void File_Io::set_32(const int32_t & in) throw (Error)
{
    set_32(&in, 1);
}

inline void File_Io::set_u32(const uint32_t & in) throw (Error)
{
    set_u32(&in, 1);
}

inline void File_Io::set_f32(const float & in) throw (Error)
{
    set_f32(&in, 1);
}

inline const uint8_t * File_Io::mmap_p() const
{
    return _mmap_p;
}

inline const uint8_t * File_Io::mmap_end() const
{
    return _mmap_end;
}

inline bool File_Io::endian() const
{
    return _endian;
}

} // djv

