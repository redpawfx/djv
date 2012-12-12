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

//! \file djv_dpx_header.h

#ifndef DJV_DPX_HEADER_H
#define DJV_DPX_HEADER_H

#include <djv_dpx.h>

namespace djv_dpx
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Header
//!
//! This class proivdes a DPX file header.
//------------------------------------------------------------------------------

class Header
{
public:

    static const char magic [][5];

    //! Image orientation.

    enum ORIENT
    {
        ORIENT_LEFT_RIGHT_TOP_BOTTOM,
        ORIENT_RIGHT_LEFT_TOP_BOTTOM,
        ORIENT_LEFT_RIGHT_BOTTOM_TOP,
        ORIENT_RIGHT_LEFT_BOTTOM_TOP,
        ORIENT_TOP_BOTTOM_LEFT_RIGHT,
        ORIENT_TOP_BOTTOM_RIGHT_LEFT,
        ORIENT_BOTTOM_TOP_LEFT_RIGHT,
        ORIENT_BOTTOM_TOP_RIGHT_LEFT
    };

    //! Descriptor.

    enum DESCRIPTOR
    {
        DESCRIPTOR_USER            = 0,
        DESCRIPTOR_R               = 1,
        DESCRIPTOR_G               = 2,
        DESCRIPTOR_B               = 3,
        DESCRIPTOR_A               = 4,
        DESCRIPTOR_L               = 6,
        DESCRIPTOR_CHROMA          = 7,
        DESCRIPTOR_Z               = 8,
        DESCRIPTOR_COMPOSITE_VIDEO = 9,
        DESCRIPTOR_RGB             = 50,
        DESCRIPTOR_RGBA            = 51,
        DESCRIPTOR_ABGR            = 52,
        DESCRIPTOR_CBYCRY          = 100,
        DESCRIPTOR_CBYACRYA        = 101,
        DESCRIPTOR_CBYCR           = 102,
        DESCRIPTOR_CBYCRA          = 103,
        DESCRIPTOR_USER_2          = 150,
        DESCRIPTOR_USER_3          = 151,
        DESCRIPTOR_USER_4          = 152,
        DESCRIPTOR_USER_5          = 153,
        DESCRIPTOR_USER_6          = 154,
        DESCRIPTOR_USER_7          = 155,
        DESCRIPTOR_USER_8          = 156
    };

    //! Transfer.

    enum TRANSFER
    {
        TRANSFER_USER,
        TRANSFER_FILM_PRINT,
        TRANSFER_LINEAR,
        TRANSFER_LOG,
        TRANSFER_VIDEO,
        TRANSFER_SMPTE_274M,
        TRANSFER_ITU_R_709_4,
        TRANSFER_ITU_R_601_5_B_OR_G,
        TRANSFER_ITU_R_601_5_M,
        TRANSFER_NTSC,
        TRANSFER_PAL,
        TRANSFER_Z,
        TRANSFER_Z_HOMOGENEOUS
    };

    //! Colorimetric vesion 1.0.

    enum COLORIMETRIC_1_0
    {
        COLORIMETRIC_1_0_USER               = 0,
        COLORIMETRIC_1_0_FILM_PRINT         = 1,
        COLORIMETRIC_1_0_VIDEO              = 4,
        COLORIMETRIC_1_0_SMPTE_240M         = 5,
        COLORIMETRIC_1_0_ITU_R_709_1        = 6,
        COLORIMETRIC_1_0_ITU_R_601_2_B_OR_G = 7,
        COLORIMETRIC_1_0_ITU_R_601_2_M      = 8,
        COLORIMETRIC_1_0_NTSC               = 9,
        COLORIMETRIC_1_0_PAL                = 10
    };

    //! Colorimetric vesion 2.0.

    enum COLORIMETRIC_2_0
    {
        COLORIMETRIC_2_0_USER               = 0,
        COLORIMETRIC_2_0_FILM_PRINT         = 1,
        COLORIMETRIC_2_0_VIDEO              = 4,
        COLORIMETRIC_2_0_SMPTE_274M         = 5,
        COLORIMETRIC_2_0_ITU_R_709_4        = 6,
        COLORIMETRIC_2_0_ITU_R_601_5_B_OR_G = 7,
        COLORIMETRIC_2_0_ITU_R_601_5_M      = 8,
        COLORIMETRIC_2_0_NTSC               = 9,
        COLORIMETRIC_2_0_PAL                = 10
    };

    //! Components.

    enum COMPONENTS
    {
        PACK,
        TYPE_A,
        TYPE_B
    };

    //! This struct provides file information.

    struct File
    {
        uint32_t magic;
        uint32_t image_offset;
        char     version [8];
        uint32_t size;
        uint32_t ditto_key;
        uint32_t header_size;
        uint32_t industry_header_size;
        uint32_t user_header_size;
        char     name [100];
        char     time [24];
        char     creator [100];
        char     project [200];
        char     copyright [200];
        uint32_t encryption_key;
        uint8_t _pad [104];

    } file;

    //! This struct provides image information.

    struct Image
    {
        uint16_t orient;
        uint16_t elem_size;
        uint32_t size [2];

        //! This struct provides image element information.

        struct Elem
        {
            uint32_t data_sign;
            uint32_t low_data;
            float    low_quantity;
            uint32_t high_data;
            float    high_quantity;
            uint8_t  descriptor;
            uint8_t  transfer;
            uint8_t  colorimetric;
            uint8_t  bit_depth;
            uint16_t packing;
            uint16_t encoding;
            uint32_t data_offset;
            uint32_t line_padding;
            uint32_t elem_padding;
            char     description [32];

        } elem [8];

        uint8_t _pad [52];

    } image;

    //! This struct provides source information.

    struct Source
    {
        uint32_t offset [2];
        float    center [2];
        uint32_t size [2];
        char     file [100];
        char     time [24];
        char     input_device [32];
        char     input_serial [32];
        uint16_t border [4];
        uint32_t pixel_aspect [2];
        float    scan_size [2]; // V2.0
        uint8_t  _pad [20];

    } source;

    //! This struct provides film information.

    struct Film
    {
        char     id [2];
        char     type [2];
        char     offset [2];
        char     prefix [6];
        char     count [4];
        char     format [32];
        uint32_t frame;
        uint32_t sequence;
        uint32_t hold;
        float    frame_rate;
        float    shutter;
        char     frame_id [32];
        char     slate [100];
        uint8_t  _pad [56];

    } film;

    //! This struct provides TV information.

    struct Tv
    {
        uint32_t timecode;
        uint32_t user_bits;
        uint8_t  interlace;
        uint8_t  field;
        uint8_t  video_signal;
        uint8_t  _pad;
        float    sample_rate [2];
        float    frame_rate;
        float    time_offset;
        float    gamma;
        float    black_level;
        float    black_gain;
        float    breakpoint;
        float    white_level;
        float    integration_times;
        uint8_t  _pad2 [76];

    } tv;

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
        ENDIAN,
        djv_cineon::COLOR_PROFILE,
        VERSION) throw (Error);

    //! Zero memory.

    static void zero(char *, int size);

    //! Get whether the value is valid.

    static bool is_valid(const uint8_t *);

    //! Get whether the value is valid.

    static bool is_valid(const uint16_t *);

    //! Get whether the value is valid.

    static bool is_valid(const uint32_t *);

    //! Get whether the value is valid.

    static bool is_valid(const float *);

    //! Get whether the value is valid.

    static bool is_valid(const char *, int size);

    //! Get whether the value is valid.

    static String to_string(const char *, int size);

    //! Load information.

    static void info_load(File_Io &, Image_Io_Info &, bool & film_print)
    throw (Error);

    //! Save information.

    static void info_save(
        File_Io &,
        const Image_Io_Info &,
        ENDIAN,
        djv_cineon::COLOR_PROFILE,
        VERSION) throw (Error);

    //! Update information.

    static void info_update(File_Io &) throw (Error);

    String debug() const;

    static String debug(uint8_t);
    static String debug(uint16_t);
    static String debug(uint32_t);
    static String debug(float);
    static String debug(const char *, int size);

private:

    void endian();
};

} // djv_dpx

#endif // DJV_DPX_HEADER_H

