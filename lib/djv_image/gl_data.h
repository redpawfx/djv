/*------------------------------------------------------------------------------
 include/djv_image/gl_data.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_GL_DATA_H
#define DJV_IMAGE_GL_DATA_H

#include <djv_image/data_def.h>

#include <djv_image/color.h>
#include <djv_image/color_profile.h>
#include <djv_image/data.h>
#include <djv_image/gl.h>
#include <djv_image/gl_offscreen_def.h>
#include <djv_base/box.h>
#include <djv_base/matrix.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::gl_data
//
//! OpenGL image data.
//------------------------------------------------------------------------------

namespace gl_data {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Transform options.

struct _DJV_IMAGE Xform
{
  Xform() :
    scale(1.0),
    rotate(0.0)
  {}

  V2b mirror;
  V2f position;
  V2f scale;
  double rotate;
};

//! Color options.
  
struct _DJV_IMAGE Color
{
  Color() :
    brightness(1.0),
    contrast(1.0),
    saturation(1.0)
  {}

  double brightness;
  double contrast;
  double saturation;
};

//! Color levels options.

struct _DJV_IMAGE Levels
{
  Levels() :
    in_low(0.0),
    in_high(1.0),
    gamma(1.0),
    out_low(0.0),
    out_high(1.0)
  {}

  double in_low, in_high;
  double gamma;
  double out_low, out_high;
};

//! Display profile options.

struct _DJV_IMAGE Display_Profile
{
  Display_Profile() :
    soft_clip(0.0)
  {}

  data::Data lut;
  Color color;
  Levels levels;
  double soft_clip;
};

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

//! Image filtering.

enum FILTER
{
  FILTER_NEAREST,
  FILTER_LINEAR,
  FILTER_BOX,
  FILTER_TRIANGLE,
  FILTER_BELL,
  FILTER_BSPLINE,
  FILTER_LANCZOS3,
  FILTER_CUBIC,
  FILTER_MITCHELL,

  _FILTER_SIZE
};

struct _DJV_IMAGE Filter
{
  Filter();
  
  Filter(FILTER min, FILTER mag);
  
  FILTER min, mag;
};

//! Options.

struct _DJV_IMAGE Option
{
  Option() :
    channel(CHANNEL_DEFAULT),
    clamp(false)
  {}

  Xform xform;
  
  color_profile::Color_Profile color_profile;
  Display_Profile display_profile;
  
  CHANNEL channel;
  
  Filter filter;
  
  color::Color background;
  
  // XXX NV_float_buffer doesn't clamp glReadPixels, so we have to do it
  // manually for integer formats.
  //
  // References:
  //
  // * http://oss.sgi.com/projects/ogl-sample/registry/NV/float_buffer.txt
  
  bool clamp;
};

//@}
//------------------------------------------------------------------------------
//!\class Data
//
//! OpenGL image data.
//------------------------------------------------------------------------------

class _DJV_IMAGE Data
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Data();
  
  ~Data();

  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  //! Draw pixels.
  
  void draw(
    const data::Data &,
    const Option & = Option(),
    bool copy = true
  ) const throw (Error);
  
  //! Read pixels.
  
  static void read(data::Data *);
  static void read(data::Data *, const Box2i &);
  
  //@}
  
private:

  Option _option;

  struct _Data;
  std::auto_ptr<_Data> _p;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Setup OpenGL state for image drawing.

_DJV_IMAGE void state_unpack(
  const data::Info &,
  const V2i & offset = V2i()
);

//! Setup OpenGL state for image reading.

_DJV_IMAGE void state_pack(
  const data::Info &,
  const V2i & offset = V2i()
);

//! Reset OpenGL state.

_DJV_IMAGE void state_reset();

//! Copy image data.

_DJV_IMAGE void copy(
  const data::Data &,
  data::Data *,
  const Option & = Option(),
  const Data * = 0,
  gl_offscreen::Buffer * = 0,
  bool copy = true
) throw (Error);

//! Create a transform matrix.

_DJV_IMAGE M3f xform_matrix(const Xform &);

//! Create a color matrix.

_DJV_IMAGE M4f brightness_matrix(double r, double g, double b);
_DJV_IMAGE M4f contrast_matrix(double r, double g, double b);
_DJV_IMAGE M4f saturation_matrix(double r, double g, double b);

_DJV_IMAGE M4f color_matrix(const Color &);

//! Create a lookup table from color levels.

_DJV_IMAGE data::Data color_lut(const Levels &, double soft_clip);

//! Calculate the average color.

_DJV_IMAGE void average(
  const data::Data &,
  color::Color *
) throw (Error);

//! Histogram size.

enum HISTOGRAM
{
  HISTOGRAM_256,
  HISTOGRAM_1024,
  
  _HISTOGRAM_SIZE
};

//! Calculate the histogram.

_DJV_IMAGE void histogram(
  const data::Data &,
  data::Data *,
  HISTOGRAM,
  color::Color * min,
  color::Color * max
) throw (Error);

_DJV_IMAGE int histogram_size(HISTOGRAM);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Xform &, const Xform &);
_DJV_IMAGE bool operator == (const Color &, const Color &);
_DJV_IMAGE bool operator == (const Levels &, const Levels &);
_DJV_IMAGE bool operator == (const Display_Profile &, const Display_Profile &);
_DJV_IMAGE bool operator == (const Filter &, const Filter &);
_DJV_IMAGE bool operator == (const Option &, const Option &);

_DJV_IMAGE bool operator != (const Xform &, const Xform &);
_DJV_IMAGE bool operator != (const Color &, const Color &);
_DJV_IMAGE bool operator != (const Levels &, const Levels &);
_DJV_IMAGE bool operator != (const Display_Profile &, const Display_Profile &);
_DJV_IMAGE bool operator != (const Filter &, const Filter &);
_DJV_IMAGE bool operator != (const Option &, const Option &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE GLuint filter_to_gl(FILTER);

_DJV_IMAGE String & operator >> (String &, Xform &) throw (String);
_DJV_IMAGE String & operator >> (String &, Color &) throw (String);
_DJV_IMAGE String & operator >> (String &, Levels &) throw (String);
_DJV_IMAGE String & operator >> (String &, Filter &) throw (String);
_DJV_IMAGE String & operator >> (String &, CHANNEL &) throw (String);
_DJV_IMAGE String & operator >> (String &, FILTER &) throw (String);
_DJV_IMAGE String & operator >> (String &, HISTOGRAM &) throw (String);

_DJV_IMAGE String & operator << (String &, const Xform &);
_DJV_IMAGE String & operator << (String &, const Color &);
_DJV_IMAGE String & operator << (String &, const Levels &);
_DJV_IMAGE String & operator << (String &, const Filter &);
_DJV_IMAGE String & operator << (String &, CHANNEL);
_DJV_IMAGE String & operator << (String &, FILTER);
_DJV_IMAGE String & operator << (String &, HISTOGRAM);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_channel();
_DJV_IMAGE const List<String> & label_filter();
_DJV_IMAGE const List<String> & label_histogram();

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, const Xform &);
_DJV_IMAGE Debug & operator << (Debug &, const Color &);
_DJV_IMAGE Debug & operator << (Debug &, const Levels &);
_DJV_IMAGE Debug & operator << (Debug &, const Filter &);
_DJV_IMAGE Debug & operator << (Debug &, CHANNEL);
_DJV_IMAGE Debug & operator << (Debug &, FILTER);
_DJV_IMAGE Debug & operator << (Debug &, HISTOGRAM);

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------

//! Global options.

class _DJV_IMAGE Global
{
public:

  Global();
  
  ~Global();

  void filter(const Filter &);
  
  const Filter & filter() const;

private:

  Filter _filter;
};

//! Access to global options.

_DJV_IMAGE Global * global();

//------------------------------------------------------------------------------

}
}

#endif

