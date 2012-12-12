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

//! \file djv_view_image.h

#ifndef DJV_VIEW_IMAGE_H
#define DJV_VIEW_IMAGE_H

#include <djv_file.h>
#include <djv_image.h>

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Image
//!
//! This class provides basic image functionality.
//------------------------------------------------------------------------------

class Image
{
public:

    //! Image scale.

    enum SCALE
    {
        SCALE_NONE,
        SCALE_16_9,
        SCALE_1_85,
        SCALE_2_35,

        _SCALE_SIZE
    };

    //! Get image scale labels.

    static const List<String> & label_scale();

    //! Calculate an image scale.

    static V2f scale(SCALE, const V2i &);

    //! Image rotate.

    enum ROTATE
    {
        ROTATE_0,
        ROTATE_90,
        ROTATE_180,
        ROTATE_270,

        _ROTATE_SIZE
    };

    //! Get image rotate labels.

    static const List<String> & label_rotate();

    //! Get the rotation in degrees.

    static double rotate(ROTATE);

    //! Load a LUT.

    static void lut_load(const File &, Pixel_Data & lut);
};

//------------------------------------------------------------------------------

String & operator >> (String &, Image::SCALE &) throw (String);
String & operator >> (String &, Image::ROTATE &) throw (String);

String & operator << (String &, Image::SCALE);
String & operator << (String &, Image::ROTATE);

} // djv_view

#endif // DJV_VIEW_IMAGE_H

