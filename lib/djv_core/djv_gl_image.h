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

//! \file djv_gl_image.h

#ifndef DJV_GL_IMAGE_H
#define DJV_GL_IMAGE_H

#include <djv_color.h>
#include <djv_color_profile.h>
#include <djv_gl.h>
#include <djv_pixel_data.h>

#include <djv_box.h>
#include <djv_error.h>
#include <djv_matrix.h>

namespace djv
{

class Gl_Image_Lut;
class Gl_Image_Shader;
class Gl_Image_Texture;
class Gl_Offscreen_Buffer;

//------------------------------------------------------------------------------
//! \struct Gl_Image_Xform
//!
//! This struct provides OpenGL image transform options.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Gl_Image_Xform
{
    //! Constructor.

    Gl_Image_Xform();

    V2b    mirror;
    V2f    position;
    V2f    scale;
    double rotate;

    //! Create a transform matrix.

    static M3f xform_matrix(const Gl_Image_Xform &);
};

//------------------------------------------------------------------------------
//! \struct Gl_Image_Color
//!
//! This struct provides OpenGL image color options.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Gl_Image_Color
{
    //! Constructor.

    Gl_Image_Color();

    double brightness;
    double contrast;
    double saturation;

    //! Create a brightness matrix.

    static M4f brightness_matrix(double r, double g, double b);

    //! Create a contrast matrix.

    static M4f contrast_matrix(double r, double g, double b);

    //! Create a saturation matrix.

    static M4f saturation_matrix(double r, double g, double b);

    //! Create a color matrix.

    static M4f color_matrix(const Gl_Image_Color &);
};

//------------------------------------------------------------------------------
//! \struct Gl_Image_Levels
//!
//! This struct provides OpenGL image color levels options.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Gl_Image_Levels
{
    //! Constructor.

    Gl_Image_Levels();

    double in_low;
    double in_high;
    double gamma;
    double out_low;
    double out_high;

    //! Create a lookup table from color levels.

    static Pixel_Data color_lut(const Gl_Image_Levels &, double soft_clip);
};

//------------------------------------------------------------------------------
//! \struct Gl_Image_Display_Profile
//!
//! This struct provides OpenGL image display profile options.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Gl_Image_Display_Profile
{
    //! Constructor.

    Gl_Image_Display_Profile();

    Pixel_Data      lut;
    Gl_Image_Color  color;
    Gl_Image_Levels levels;
    double          soft_clip;
};

//------------------------------------------------------------------------------
//! \struct Gl_Image_Filter
//!
//! This struct provides OpenGL image filtering options.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Gl_Image_Filter
{
    //! Image filtering.

    enum FILTER
    {
        NEAREST,
        LINEAR,
        BOX,
        TRIANGLE,
        BELL,
        BSPLINE,
        LANCZOS3,
        CUBIC,
        MITCHELL,

        _FILTER_SIZE
    };

    //! Get the image filtering labels.

    static const List<String> & label_filter();

    //! Constructor.

    Gl_Image_Filter();

    //! Constructor.

    Gl_Image_Filter(FILTER min, FILTER mag);

    FILTER min;
    FILTER mag;

    //! Convert a filter to OpenGL.

    static GLuint filter_to_gl(FILTER);

    //! The default filter.

    static Gl_Image_Filter default_filter;
};

//------------------------------------------------------------------------------
//! \struct Gl_Image_Options
//!
//! This struct provides OpenGL image options.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Gl_Image_Options
{
    //! Channels to display.

    enum CHANNEL
    {
        CHANNEL_DEFAULT,
        CHANNEL_RED,
        CHANNEL_GREEN,
        CHANNEL_BLUE,
        CHANNEL_ALPHA,

        _CHANNEL_SIZE
    };

    //! Get the channel labels.

    static const List<String> & label_channel();

    //! Constructor.

    Gl_Image_Options();

    Gl_Image_Xform           xform;
    Color_Profile            color_profile;
    Gl_Image_Display_Profile display_profile;
    CHANNEL                  channel;
    Gl_Image_Filter          filter;
    Color                    background;
    bool                     proxy_scale;
};

//------------------------------------------------------------------------------
//! \struct Gl_Image_State
//!
//! This struct provides OpenGL image state.
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Gl_Image_State
{
    //! Constructor.

    Gl_Image_State();

    //! Destructor.

    ~Gl_Image_State();

private:

    bool               _init;
    Pixel_Data_Info    _info;
    Gl_Image_Options   _options;
    Gl_Image_Texture * _texture;
    Gl_Image_Shader  * _shader;
    Gl_Image_Texture * _scale_x_contrib;
    Gl_Image_Texture * _scale_y_contrib;
    Gl_Image_Shader  * _scale_x_shader;
    Gl_Image_Shader  * _scale_y_shader;
    Gl_Image_Lut *     _lut_color_profile;
    Gl_Image_Lut *     _lut_display_profile;

    friend class Gl_Image;
};

//------------------------------------------------------------------------------
//! \class Gl_Image
//!
//! This class provides OpenGL image utilities.
//------------------------------------------------------------------------------

class DJV_CORE_EXPORT Gl_Image
{
public:

    //! Draw pixel data.

    static void draw(
        const Pixel_Data &       data,
        const Gl_Image_Options & options = Gl_Image_Options(),
        Gl_Image_State *         state   = 0) throw (Error);

    //! Read pixel data.

    static void read(Pixel_Data &);

    //! Read pixel data.

    static void read(Pixel_Data &, const Box2i &);

    //! Copy image data.

    static void copy(
        const Pixel_Data &       input,
        Pixel_Data &             output,
        const Gl_Image_Options & options = Gl_Image_Options(),
        Gl_Image_State *         state   = 0,
        Gl_Offscreen_Buffer *    buffer  = 0) throw (Error);

    //! Setup OpenGL state for image drawing.

    static void state_unpack(
        const Pixel_Data_Info &,
        const V2i & offset = V2i());

    //! Setup OpenGL state for image reading.

    static void state_pack(
        const Pixel_Data_Info &,
        const V2i & offset = V2i());

    //! Reset OpenGL state.

    static void state_reset();

    //! Calculate the average color.

    static void average(const Pixel_Data &, Color *) throw (Error);

    //! Histogram size.

    enum HISTOGRAM
    {
        HISTOGRAM_256,
        HISTOGRAM_1024,

        _HISTOGRAM_SIZE
    };

    //! Get the histogram size.

    static int histogram_size(HISTOGRAM);

    //! Get the histogram labels.

    static const List<String> & label_histogram();

    //! Calculate the histogram.

    static void histogram(
        const Pixel_Data &,
        Pixel_Data *,
        HISTOGRAM,
        Color * min,
        Color * max) throw (Error);
};

//------------------------------------------------------------------------------

DJV_CORE_EXPORT bool operator == (
    const Gl_Image_Xform &, const Gl_Image_Xform &);
DJV_CORE_EXPORT bool operator == (
    const Gl_Image_Color &, const Gl_Image_Color &);
DJV_CORE_EXPORT bool operator == (
    const Gl_Image_Levels &, const Gl_Image_Levels &);
DJV_CORE_EXPORT bool operator == (
    const Gl_Image_Display_Profile &, const Gl_Image_Display_Profile &);
DJV_CORE_EXPORT bool operator == (
    const Gl_Image_Filter &, const Gl_Image_Filter &);
DJV_CORE_EXPORT bool operator == (
    const Gl_Image_Options &, const Gl_Image_Options &);

DJV_CORE_EXPORT bool operator != (
    const Gl_Image_Xform &, const Gl_Image_Xform &);
DJV_CORE_EXPORT bool operator != (
    const Gl_Image_Color &, const Gl_Image_Color &);
DJV_CORE_EXPORT bool operator != (
    const Gl_Image_Levels &, const Gl_Image_Levels &);
DJV_CORE_EXPORT bool operator != (
    const Gl_Image_Display_Profile &, const Gl_Image_Display_Profile &);
DJV_CORE_EXPORT bool operator != (
    const Gl_Image_Filter &, const Gl_Image_Filter &);
DJV_CORE_EXPORT bool operator != (
    const Gl_Image_Options &, const Gl_Image_Options &);

DJV_CORE_EXPORT String & operator >> (String &, Gl_Image_Xform &)
    throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Gl_Image_Color &)
    throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Gl_Image_Levels &)
    throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Gl_Image_Filter &)
    throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Gl_Image_Filter::FILTER &)
    throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Gl_Image_Options::CHANNEL &)
    throw (String);
DJV_CORE_EXPORT String & operator >> (String &, Gl_Image::HISTOGRAM &)
    throw (String);

DJV_CORE_EXPORT String & operator << (String &, const Gl_Image_Xform &);
DJV_CORE_EXPORT String & operator << (String &, const Gl_Image_Color &);
DJV_CORE_EXPORT String & operator << (String &, const Gl_Image_Levels &);
DJV_CORE_EXPORT String & operator << (String &, const Gl_Image_Filter &);
DJV_CORE_EXPORT String & operator << (String &, Gl_Image_Filter::FILTER);
DJV_CORE_EXPORT String & operator << (String &, Gl_Image_Options::CHANNEL);
DJV_CORE_EXPORT String & operator << (String &, Gl_Image::HISTOGRAM);

DJV_CORE_EXPORT Debug & operator << (Debug &, const Gl_Image_Xform &);
DJV_CORE_EXPORT Debug & operator << (Debug &, const Gl_Image_Color &);
DJV_CORE_EXPORT Debug & operator << (Debug &, const Gl_Image_Levels &);
DJV_CORE_EXPORT Debug & operator << (Debug &, const Gl_Image_Filter &);
DJV_CORE_EXPORT Debug & operator << (Debug &, Gl_Image_Filter::FILTER);
DJV_CORE_EXPORT Debug & operator << (Debug &, Gl_Image_Options::CHANNEL);
DJV_CORE_EXPORT Debug & operator << (Debug &, Gl_Image::HISTOGRAM);

} // djv

#endif // DJV_GL_IMAGE_H

