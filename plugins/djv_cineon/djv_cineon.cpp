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

//! \file djv_cineon.cpp

#include <djv_cineon.h>

#include <djv_assert.h>

namespace djv_cineon
{

const List<String> & label_color_profile()
{
    static const List<String> data = List<String>() <<
        "Auto" <<
        "None" <<
        "Film Print";

    DJV_ASSERT(data.size() == _COLOR_PROFILE_SIZE);

    return data;
}

const List<String> & label_tag()
{
    static const List<String> data = List<String>() <<
        "Source Offset" <<
        "Source File" <<
        "Source Time" <<
        "Source Input Device" <<
        "Source Input Model" <<
        "Source Input Serial" <<
        "Source Input Pitch" <<
        "Source Gamma" <<
        "Film Format" <<
        "Film Frame" <<
        "Film Frame Rate" <<
        "Film Frame ID" <<
        "Film Slate";

    DJV_ASSERT(data.size() == _TAG_SIZE);

    return data;
}

const List<String> & label_convert()
{
    static const List<String> data = List<String>() <<
        "None" <<
        "U8" <<
        "U16";

    DJV_ASSERT(data.size() == _CONVERT_SIZE);

    return data;
}

//! \todo Preserve black and white points?

void film_print(Pixel::F32_T * in, int size, const Film_Print & value)
{
    //DJV_DEBUG("film_print");
    //DJV_DEBUG_PRINT("size = " << size);
    //DJV_DEBUG_PRINT("black = " << value.black);
    //DJV_DEBUG_PRINT("white = " << value.white);
    //DJV_DEBUG_PRINT("gamma = " << value.gamma);

    const double gain =
        1.0 / (
            1.0 - Math::pow(
                Math::pow(10.0, (value.black - value.white) * 0.002 / 0.6),
                value.gamma / 1.7));

    const double offset = gain - 1.0;

    //DJV_DEBUG_PRINT("gain = " << gain * 255);
    //DJV_DEBUG_PRINT("offset = " << offset * 255);

    for (int i = 0; i < size; ++i)
    {
        in[i] = i / Pixel::F32_T(size - 1);
    }

    for (int i = 0; i < size; ++i)
    {
        in[i] = Pixel::F32_T(
            value.white / 1023.0 +
            Math::log_10(
                Math::pow((in[i] + offset) / gain, 1.7 / value.gamma)) /
                    (2.048 / 0.6));

        //DJV_DEBUG_PRINT("lut[" << i << "] = " <<
        //    in[i] << " " << static_cast<int>(in[i] * 1024));
    }
}

Pixel_Data film_print_lut(const Film_Print & in)
{
    Pixel_Data out(Pixel_Data_Info(V2i(1024, 1), Pixel::L_F32));

    film_print(reinterpret_cast<Pixel::F32_T *>(out.data()), out.w(), in);

    return out;
}

void film_unprint(Pixel::F32_T * in, int size, const Film_Unprint & value)
{
    //DJV_DEBUG("film_unprint");
    //DJV_DEBUG_PRINT("black = " << value.black);
    //DJV_DEBUG_PRINT("white = " << value.white);
    //DJV_DEBUG_PRINT("gamma = " << value.gamma);
    //DJV_DEBUG_PRINT("soft clip = " << value.soft_clip);

    const double gain =
        1.0 / (
            1.0 - Math::pow(
                Math::pow(10.0, (value.black - value.white) * 0.002 / 0.6),
                value.gamma / 1.7));

    const double offset = gain - 1.0;

    //DJV_DEBUG_PRINT("gain = " << gain * 255);
    //DJV_DEBUG_PRINT("offset = " << offset * 255);

    const int break_point = value.white - value.soft_clip;

    const double knee_offset =
        Math::pow(
            Math::pow(10.0, (break_point - value.white) * 0.002 / 0.6),
            value.gamma / 1.7
        ) *
        gain - offset;

    const double knee_gain =
        (
            (255 - (knee_offset * 255)) /
            Math::pow(5.0 * value.soft_clip, value.soft_clip / 100.0)
        ) / 255.0;

    //DJV_DEBUG_PRINT("break point = " << break_point);
    //DJV_DEBUG_PRINT("knee offset = " << knee_offset * 255);
    //DJV_DEBUG_PRINT("knee gain = " << knee_gain * 255);

    for (int i = 0; i < size; ++i)
    {
        in[i] = i / Pixel::F32_T(size - 1);
    }

    for (int i = 0; i < size; ++i)
    {
        const int tmp = static_cast<int>(in[i] * 1023.0);

        if (tmp < value.black)
        {
            in[i] = 0.0;
        }
        else if (tmp > break_point)
        {
            in[i] = Pixel::F32_T((Math::pow(
                static_cast<double>(tmp - break_point),
                value.soft_clip / 100.0) *
                knee_gain * 255 + knee_offset * 255) / 255.0);
        }
        else
        {
            in[i] = Pixel::F32_T(Math::pow(
                Math::pow(10.0, (tmp - value.white) * 0.002 / 0.6),
                value.gamma / 1.7) * gain - offset);
        }

        //DJV_DEBUG_PRINT("lut[" << i << "] = " <<
        //  in[i] << " " << static_cast<int>(in[i] * 255.0));
    }
}

Pixel_Data film_unprint_lut(const Film_Unprint & in)
{
    Pixel_Data out(Pixel_Data_Info(V2i(1024, 1), Pixel::L_F32));

    film_unprint(reinterpret_cast<Pixel::F32_T *>(out.data()), out.w(), in);

    return out;
}

bool operator == (const Film_Print & a, const Film_Print & b)
{
    return
        a.black == b.black &&
        a.white == b.white &&
        a.gamma == b.gamma;
}

bool operator == (const Film_Unprint & a, const Film_Unprint & b)
{
    return
        a.black == b.black &&
        a.white == b.white &&
        a.gamma == b.gamma &&
        a.soft_clip == b.soft_clip;
}

String & operator >> (String & in, Film_Print & out) throw (String)
{
    return in >>
           out.black >>
           out.white >>
           out.gamma;
}

String & operator << (String & out, const Film_Print & in)
{
    return out <<
           in.black <<
           in.white <<
           in.gamma;
}

String & operator >> (String & in, Film_Unprint & out) throw (String)
{
    return in >>
           out.black >>
           out.white >>
           out.gamma >>
           out.soft_clip;
}

String & operator << (String & out, const Film_Unprint & in)
{
    return out <<
           in.black <<
           in.white <<
           in.gamma <<
           in.soft_clip;
}

_DJV_STRING_OPERATOR_LABEL(COLOR_PROFILE, label_color_profile())
_DJV_STRING_OPERATOR_LABEL(CONVERT, label_convert())


} // djv_cineon

