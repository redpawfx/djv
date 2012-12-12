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

//! \file djv_view_image.cpp

#include <djv_view_image.h>

#include <djv_application.h>

#include <djv_assert.h>
#include <djv_image_io.h>

namespace djv_view
{

//------------------------------------------------------------------------------
// Image
//------------------------------------------------------------------------------

const List<String> & Image::label_scale()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "16.9" <<
        "1.85" <<
        "2.35";

    DJV_ASSERT(data.size() == _SCALE_SIZE);

    return data;
}

V2f Image::scale(SCALE in, const V2i & size)
{
    static const double data [] = { 1.0, 16.0 / 9.0, 1.85, 2.35 };

    V2f out(1.0);

    switch (in)
    {
        case SCALE_16_9:
        case SCALE_1_85:
        case SCALE_2_35:
            out.x = data[in] / Vector_Util::aspect(size);
            break;

        default:
            break;
    }

    return out;
}

const List<String> & Image::label_rotate()
{
    static const List<String> data = List<String>() <<
        "0" <<
        "90" <<
        "180" <<
        "270";

    DJV_ASSERT(data.size() == _ROTATE_SIZE);

    return data;
}

double Image::rotate(ROTATE in)
{
    static const double data [] = { 0, 90, 180, 270 };

    return data[in];
}

void Image::lut_load(const File & in, Pixel_Data & lut)
{
    if (in.get().empty())
    {
        return;
    }

    //DJV_DEBUG("Image::lut_load");
    //DJV_DEBUG_PRINT("in = " << in);

    File file(in);

    if (file.is_seq_valid())
    {
        file.type(File::SEQ);
    }

    try
    {
        djv::Image image;
        
        Image_Load_Factory::global()->get(file)->load(image);
        
        lut = image;

        //DJV_DEBUG_PRINT("lut = " << lut);
    }
    catch (Error error)
    {
        DJV_APP->error(error);
    }
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Image::SCALE, Image::label_scale())
_DJV_STRING_OPERATOR_LABEL(Image::ROTATE, Image::label_rotate())

} // djv_view
