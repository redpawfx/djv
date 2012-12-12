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

//! \file djv_cineon_header.h

#ifndef DJV_CINEON_HEADER_H
#define DJV_CINEON_HEADER_H

#include <djv_cineon.h>

namespace djv_cineon
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Header
//!
//! This class provides Cineon file header functionality.
//------------------------------------------------------------------------------

class Header
{
public:

    static const uint32_t magic [];

    static const float speed_min;

    //! Image orientation.

    enum ORIENT
    {
        ORIENT_LEFT_RIGHT_TOP_BOTTOM,
        ORIENT_LEFT_RIGHT_BOTTOM_TOP,
        ORIENT_RIGHT_LEFT_TOP_BOTTOM,
        ORIENT_RIGHT_LEFT_BOTTOM_TOP,
        ORIENT_TOP_BOTTOM_LEFT_RIGHT,
        ORIENT_TOP_BOTTOM_RIGHT_LEFT,
        ORIENT_BOTTOM_TOP_LEFT_RIGHT,
        ORIENT_BOTTOM_TOP_RIGHT_LEFT
    };

    //! Descriptor.

    enum DESCRIPTOR
    {
        DESCRIPTOR_L = 0,
        DESCRIPTOR_R_FILM_PRINT = 1,
        DESCRIPTOR_G_FILM_PRINT = 2,
        DESCRIPTOR_B_FILM_PRINT = 3,
        DESCRIPTOR_R_CCIR_XA11 = 4,
        DESCRIPTOR_G_CCIR_XA11 = 5,
        DESCRIPTOR_B_CCIR_XA11 = 6
    };

    //! This struct provides file information.

    struct File
    {
        uint32_t magic;
        uint32_t image_offset;
        uint32_t header_size;
        uint32_t industry_header_size;
        uint32_t user_header_size;
        uint32_t size;
        char     version [8];
        char     name [100];
        char     time [24];
        uint8_t  _pad [36];

    } file;

    //! This struct provides image information.

    struct Image
    {
        uint8_t orient;
        uint8_t channels;
        uint8_t _pad [2];

        //! This struct provides channel information.

        struct Channel
        {
            uint8_t  descriptor [2];
            uint8_t  bit_depth;
            uint8_t  _pad;
            uint32_t size [2];
            float    low_data;
            float    low_quantity;
            float    high_data;
            float    high_quantity;

        } channel [8];

        float    white [2];
        float    red [2];
        float    green [2];
        float    blue [2];
        char     label [200];
        uint8_t  _pad2 [28];
        uint8_t  interleave;
        uint8_t  packing;
        uint8_t  data_sign;
        uint8_t  data_sense;
        uint32_t line_padding;
        uint32_t channel_padding;
        uint8_t  _pad3 [20];

    } image;

    //! This struct provides source information.

    struct Source
    {
        int32_t offset [2];
        char    file [100];
        char    time [24];
        char    input_device [64];
        char    input_model [32];
        char    input_serial [32];
        float   input_pitch [2];
        float   gamma;
        char    _pad [40];

    } source;

    //! This struct provides film information.

    struct Film
    {
        uint8_t  id;
        uint8_t  type;
        uint8_t  offset;
        uint8_t  _pad;
        uint32_t prefix;
        uint32_t count;
        char     format [32];
        uint32_t frame;
        float    frame_rate;
        char     frame_id [32];
        char     slate [200];
        char     _pad2 [740];

    } film;

    //! Constructor.

    Header();

    //! Load the header.

    void load(
        File_Io &,
        Image_Io_Info &,
        bool & film_print) throw (Error);

    //! Save the header.

    void save(
        File_Io &,
        const Image_Io_Info &,
        COLOR_PROFILE) throw (Error);

    //! Zero memory.

    static void zero(int32_t *);

    //! Zero memory.

    static void zero(float *);

    //! Zero memory.

    static void zero(char *, int size);

    //! Get whether the value is valid.

    static bool is_valid(const uint8_t *);

    //! Get whether the value is valid.

    static bool is_valid(const uint16_t *);

    //! Get whether the value is valid.

    static bool is_valid(const uint32_t *);

    //! Get whether the value is valid.

    static bool is_valid(const int32_t *);

    //! Get whether the value is valid.

    static bool is_valid(const float *);

    //! Get whether the value is valid.

    static bool is_valid(const char *, int size);

    //! Convert to a string.

    static String to_string(const char *, int size);

    //! Load information.

    static void info_load(File_Io &, Image_Io_Info &, bool & film_print)
    throw (Error);

    //! Save information.

    static void info_save(File_Io &, const Image_Io_Info &, COLOR_PROFILE)
    throw (Error);

    //! Update information.

    static void info_update(File_Io &) throw (Error);

    String debug() const;

    static String debug(uint8_t);
    static String debug(uint16_t);
    static String debug(uint32_t);
    static String debug(int32_t);
    static String debug(float);
    static String debug(const char *, int size);

private:

    void endian();
};

} // djv_cineon

#endif // DJV_CINEON_HEADER_H

