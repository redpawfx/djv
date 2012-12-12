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

//! \file djv_pixel_data.h

#ifndef DJV_PIXEL_DATA_H
#define DJV_PIXEL_DATA_H

#include <djv_pixel.h>

#include <djv_box.h>
#include <djv_memory.h>
#include <djv_memory_buffer.h>
#include <djv_vector.h>

namespace djv
{

class File_Io;

//------------------------------------------------------------------------------
//! \class Pixel_Data_Info
//!
//! This class provides pixel data information.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Pixel_Data_Info
{
public:

    //! Constructor.

    Pixel_Data_Info();

    //! Constructor.

    Pixel_Data_Info(
        const V2i &  size,
        Pixel::PIXEL pixel);

    //! Constructor.

    Pixel_Data_Info(
        const String & file_name,
        const V2i &    size,
        Pixel::PIXEL   pixel);

    //! Proxy scale.

    enum PROXY
    {
        PROXY_NONE,
        PROXY_1_2,
        PROXY_1_4,
        PROXY_1_8,

        _PROXY_SIZE
    };

    //! Get the proxy scale labels.

    static const List<String> & label_proxy();

    String         file_name;
    String         layer_name;
    V2i            size;
    PROXY          proxy;
    Pixel::PIXEL   pixel;
    bool           bgr;
    V2b            mirror;
    int            align;
    Memory::ENDIAN endian;

private:

    void init();
};

//------------------------------------------------------------------------------
//! \class Pixel_Data
//!
//! This class provides pixel data.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Pixel_Data
{
public:

    //! Constructor.

    Pixel_Data();

    //! Constructor.

    Pixel_Data(const Pixel_Data &);

    //! Constructor.

    Pixel_Data(const Pixel_Data_Info &, const uint8_t * = 0, File_Io * = 0);

    //! Destructor.

    ~Pixel_Data();

    Pixel_Data & operator = (const Pixel_Data &);

    //! Copy the ppixel data.

    void set(const Pixel_Data &);

    //! Set the ppixel data.

    void set(const Pixel_Data_Info &, const uint8_t * = 0, File_Io * = 0);

    //! Zero the pixel data.

    void zero();

    //! Get the information.

    inline const Pixel_Data_Info & info() const;

    //! Get the dimensions.

    inline const V2i & size() const;

    //! Get the proxy scale.

    inline Pixel_Data_Info::PROXY proxy() const;

    //! Get the width.

    inline int w() const;

    //! Get the height.

    inline int h() const;

    //! Get the pixel.

    inline Pixel::PIXEL pixel() const;

    //! Get the number of channels.

    inline int channels() const;

    //! Get whether the pixel data is valid.

    inline bool is_valid() const;

    //! Get a pointer to the data.

    inline uint8_t * data();

    //! Get a pointer to the data.

    inline const uint8_t * data() const;

    //! Get a pointer to the data.

    inline uint8_t * data(int x, int y);

    //! Get a pointer to the data.

    inline const uint8_t * data(int x, int y) const;

    //! Get the number of bytes in a pixel.

    inline size_t bytes_pixel() const;

    //! Get the number of bytes in a scanline.

    inline size_t bytes_scanline() const;

    //! Get the number of bytes in the data.

    inline size_t bytes_data() const;

    //! Proxy scale.

    static void proxy_scale(
        const Pixel_Data &,
        Pixel_Data *,
        Pixel_Data_Info::PROXY);

    //! Calculate the proxy scale.

    static int proxy_scale(Pixel_Data_Info::PROXY);

    //! Calculate the proxy scale.

    static V2i proxy_scale(const V2i &, Pixel_Data_Info::PROXY);

    //! Calculate the proxy scale.

    static Box2i proxy_scale(const Box2i &, Pixel_Data_Info::PROXY);

    //! Get the number of bytes in a scanline.

    static size_t bytes_scanline(const Pixel_Data_Info &);

    //! Get the number of bytes in the data.

    static size_t bytes_data(const Pixel_Data_Info &);

    //! Interleave channels.

    static void planar_interleave(
        const Pixel_Data &,
        Pixel_Data *,
        Pixel_Data_Info::PROXY = Pixel_Data_Info::PROXY_NONE);

    //! De-interleave channels.

    static void planar_deinterleave(const Pixel_Data &, Pixel_Data *);

    //! Create a linear gradient.

    static void gradient(Pixel_Data *);

private:

    void init();
    void alloc();
    void del();

    Pixel_Data_Info        _info;
    int                    _channels;
    Memory_Buffer<uint8_t> _data;
    size_t                 _bytes_pixel;
    size_t                 _bytes_scanline;
    size_t                 _bytes_data;
    const uint8_t *        _p;
    File_Io *              _io;
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT bool operator == (
    const Pixel_Data_Info &,
    const Pixel_Data_Info &);
DJV_CORE_EXPORT bool operator == (const Pixel_Data &, const Pixel_Data &);

DJV_CORE_EXPORT bool operator != (
    const Pixel_Data_Info &,
    const Pixel_Data_Info &);
DJV_CORE_EXPORT bool operator != (const Pixel_Data &, const Pixel_Data &);

DJV_CORE_EXPORT String & operator >> (String &, Pixel_Data_Info::PROXY &)
    throw (String);

DJV_CORE_EXPORT String & operator << (String &, Pixel_Data_Info::PROXY);

DJV_CORE_EXPORT Debug & operator << (Debug &, Pixel_Data_Info::PROXY);
DJV_CORE_EXPORT Debug & operator << (Debug &, const Pixel_Data_Info &);
DJV_CORE_EXPORT Debug & operator << (Debug &, const Pixel_Data &);

} // djv

#include <djv_pixel_data_inline.h>

#endif // DJV_PIXEL_DATA_H

