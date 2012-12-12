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

//! \file djv_file_io.h

#ifndef DJV_FILE_IO_H
#define DJV_FILE_IO_H

#include <djv_error.h>
#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class File_Io
//!
//! This class provides file I/O.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT File_Io
{
public:

    //! Constructor.

    File_Io();

    //! Destructor.

    ~File_Io();

    //! Open mode.

    enum MODE
    {
        READ,
        WRITE
    };

    //! Open a file.

    void open(const String & file_name, MODE) throw (Error);

    //! Close the file.

    void close();

    //! Get the file name.

    const String & file_name() const;

    //! Get whether the file is open.

    bool is_valid() const;

    //! Get the file size.

    inline size_t size() const;

    //! Set data.

    void set(const void *, size_t, size_t word_size = 1) throw (Error);

    void set_8(const int8_t *, size_t) throw (Error);
    void set_u8(const uint8_t *, size_t) throw (Error);
    void set_16(const int16_t *, size_t) throw (Error);
    void set_u16(const uint16_t *, size_t) throw (Error);
    void set_32(const int32_t *, size_t) throw (Error);
    void set_u32(const uint32_t *, size_t) throw (Error);
    void set_f32(const float *, size_t) throw (Error);

    inline void set_8(const int8_t & in) throw (Error);
    inline void set_u8(const uint8_t & in) throw (Error);
    inline void set_16(const int16_t & in) throw (Error);
    inline void set_u16(const uint16_t & in) throw (Error);
    inline void set_32(const int32_t & in) throw (Error);
    inline void set_u32(const uint32_t & in) throw (Error);
    inline void set_f32(const float & in) throw (Error);

    //! Get data.

    void get(void *, size_t, size_t word_size = 1) throw (Error);

    void get_8(int8_t *, size_t = 1) throw (Error);
    void get_u8(uint8_t *, size_t = 1) throw (Error);
    void get_16(int16_t *, size_t = 1) throw (Error);
    void get_u16(uint16_t *, size_t = 1) throw (Error);
    void get_32(int32_t *, size_t = 1) throw (Error);
    void get_u32(uint32_t *, size_t = 1) throw (Error);
    void get_f32(float *, size_t = 1) throw (Error);

    //! Start an asynchronous read-ahead. This allows the operating system to
    //! cache the file by the time we need it.

    void read_ahead();

    //! Get the current memory-map position.

    inline const uint8_t * mmap_p() const;

    //! Get a pointer to the end of the memory-map.

    inline const uint8_t * mmap_end() const;

    //! Set the file position.

    void position(size_t) throw (Error);

    //! Get the file position.

    size_t position() const;

    //! Move the file position.

    void seek(size_t) throw (Error);

    //! Set whether endian conversion is performed when using the data
    //! functions.

    void endian(bool);

    //! Get whether endian conversion is performed when using the data
    //! functions.

    inline bool endian() const;

    //! Read a word from a file.

    static void word(
        File_Io &,
        char *,
        int max_len = cstring_size) throw (Error);

    //! Read a line from a file.

    static void line(
        File_Io &,
        char *,
        int max_len = cstring_size) throw (Error);

    //! Read all the lines from a file.

    static List<String> lines(const String & file_name) throw (Error);

private:

    void position(size_t, bool seek) throw (Error);

#   if defined(DJV_WINDOWS)
    HANDLE          _f;
#   else
    int             _f;
#   endif
    String          _file_name;
    MODE            _mode;
    size_t          _size;
    bool            _endian;
    void *          _mmap;
    const uint8_t * _mmap_start;
    const uint8_t * _mmap_end;
    const uint8_t * _mmap_p;
    int             _mmap_read_ahead;
};

} // djv

#include <djv_file_io_inline.h>

#endif // DJV_FILE_IO_H

