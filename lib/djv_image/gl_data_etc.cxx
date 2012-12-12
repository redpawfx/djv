/*------------------------------------------------------------------------------
 lib/djv_image/gl_data_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "gl_data.h"

namespace djv_image {
namespace gl_data {

//------------------------------------------------------------------------------
// state_unpack(), state_pack(), state_reset()
//------------------------------------------------------------------------------

void state_unpack(const Info & in, const V2i & offset)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, in.align);
  glPixelStorei(GL_UNPACK_SWAP_BYTES, in.endian != memory::endian());
  //glPixelStorei(GL_UNPACK_ROW_LENGTH, in.data_window.w);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, offset.y);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, offset.x);
}

void state_pack(const Info & in, const V2i & offset)
{
  glPixelStorei(GL_PACK_ALIGNMENT, in.align);
  glPixelStorei(GL_PACK_SWAP_BYTES, in.endian != memory::endian());
  glPixelStorei(GL_PACK_ROW_LENGTH, in.size.x);
  glPixelStorei(GL_PACK_SKIP_ROWS, offset.y);
  glPixelStorei(GL_PACK_SKIP_PIXELS, offset.x);
}

void state_reset()
{
  state_pack(Info());
  state_unpack(Info());
  if (GLEW_ARB_shader_objects) glUseProgramObjectARB(0);
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

//------------------------------------------------------------------------------
// xform_matrix()
//------------------------------------------------------------------------------

M3f xform_matrix(const Xform & in)
{
  return
    matrix::scale3f(in.scale) *
    matrix::rotate3f(in.rotate) *
    matrix::translate3f(in.position);
}

//------------------------------------------------------------------------------
// brightness_matrix(), contrast_matrix(), saturation_matrix(), color_matrix()
//------------------------------------------------------------------------------

M4f brightness_matrix(double r, double g, double b)
{
  return
    M4f(
      r, 0, 0, 0,
      0, g, 0, 0,
      0, 0, b, 0,
      0, 0, 0, 1
    );
}

M4f contrast_matrix(double r, double g, double b)
{
  return
    M4f(
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      -0.5, -0.5, -0.5, 1
    ) *
    M4f(
      r, 0, 0, 0,
      0, g, 0, 0,
      0, 0, b, 0,
      0, 0, 0, 1
    ) *
    M4f(
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0.5, 0.5, 0.5, 1
    );
}

M4f saturation_matrix(double r, double g, double b)
{
  const double s [] =
  {
    (1.0 - r) * 0.3086,
    (1.0 - g) * 0.6094,
    (1.0 - b) * 0.0820
  };
  
  return
    M4f(
      s[0] + r, s[0],     s[0],     0,
      s[1],     s[1] + g, s[1],     0,
      s[2],     s[2],     s[2] + b, 0,
      0,        0,        0,        1
    );
}

M4f color_matrix(const Color & in)
{
  return
    brightness_matrix(in.brightness, in.brightness, in.brightness) *
    contrast_matrix(in.contrast, in.contrast, in.contrast) *
    saturation_matrix(in.saturation, in.saturation, in.saturation);
}

//------------------------------------------------------------------------------
// color_lut()
//------------------------------------------------------------------------------

data::Data color_lut(const Levels & in, double soft_clip)
{
  data::Data out(Info(V2i(1024, 1), pixel::L_F32));
  
  const double in_tmp = in.in_high - in.in_low;
  const double gamma = 1.0 / in.gamma;
  const double out_tmp = in.out_high - in.out_low;

  pixel::F32_T * p = reinterpret_cast<pixel::F32_T *>(out.data());
  const int size = out.size().x;
  for (int i = 0; i < size; ++i, ++p)
    *p = static_cast<pixel::F32_T>(
      math::soft_clip(
        math::pow(
          math::max(
            ((i / static_cast<double>(size - 1) - in.in_low) / in_tmp),
            0.000001
          ),
          gamma
        ) * out_tmp + in.out_low,
        soft_clip
      )
    );
  
  return out;
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Xform & a, const Xform & b)
{
  return
    a.mirror == b.mirror &&
    a.position == b.position &&
    a.scale == b.scale &&
    a.rotate == b.rotate;
}

bool operator != (const Xform & a, const Xform & b)
{
  return ! (a == b);
}

bool operator == (const Color & a, const Color & b)
{
  return
    a.brightness == b.brightness &&
    a.contrast == b.contrast &&
    a.saturation == b.saturation;
}

bool operator != (const Color & a, const Color & b)
{
  return ! (a == b);
}

bool operator == (const Levels & a, const Levels & b)
{
  return
    a.in_low == b.in_low &&
    a.in_high == b.in_high &&
    a.gamma == b.gamma &&
    a.out_low == b.out_low &&
    a.out_high == b.out_high;
}

bool operator != (const Levels & a, const Levels & b)
{
  return ! (a == b);
}

bool operator == (const Display_Profile & a, const Display_Profile & b)
{
  return
    a.lut == b.lut &&
    a.color == b.color &&
    a.levels == b.levels &&
    a.soft_clip == b.soft_clip;
}

bool operator != (const Display_Profile & a, const Display_Profile & b)
{
  return ! (a == b);
}

bool operator == (const Filter & a, const Filter & b)
{
  return
    a.min == b.min &&
    a.mag == b.mag;
}

bool operator != (const Filter & a, const Filter & b)
{
  return ! (a == b);
}

bool operator == (const Option & a, const Option & b)
{
  return
    a.xform == b.xform &&
    a.color_profile == b.color_profile &&
    a.display_profile == b.display_profile &&
    a.channel == b.channel &&
    a.filter == b.filter &&
    a.background == b.background &&
    a.clamp == b.clamp;
}

bool operator != (const Option & a, const Option & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

GLuint filter_to_gl(FILTER in)
{
  switch (in)
  {
    case FILTER_NEAREST: return GL_NEAREST;
    case FILTER_LINEAR: return GL_LINEAR;
    default: break;
  }
  return 0;
}

String & operator >> (String & in, Xform & out) throw (String)
{
  return in >>
    out.mirror >>
    out.position >>
    out.scale >>
    out.rotate;
}

String & operator << (String & out, const Xform & in)
{
  return out <<
    in.mirror <<
    in.position <<
    in.scale <<
    in.rotate;
}

String & operator >> (String & in, Color & out) throw (String)
{
  return in >>
    out.brightness >>
    out.contrast >>
    out.saturation;
}

String & operator << (String & out, const Color & in)
{
  return out <<
    in.brightness <<
    in.contrast <<
    in.saturation;
}

String & operator >> (String & in, Levels & out) throw (String)
{
  return in >>
    out.in_low >>
    out.in_high >>
    out.gamma >>
    out.out_low >>
    out.out_high;
}

String & operator << (String & out, const Levels & in)
{
  return out <<
    in.in_low <<
    in.in_high <<
    in.gamma <<
    in.out_low <<
    in.out_high;
}

String & operator >> (String & in, Filter & out) throw (String)
{
  return in >>
    out.min >>
    out.mag;
}

String & operator << (String & out, const Filter & in)
{
  return out <<
    in.min <<
    in.mag;
}

_DJV_STRING_OPERATOR_LABEL(CHANNEL, label_channel())
_DJV_STRING_OPERATOR_LABEL(FILTER, label_filter())
_DJV_STRING_OPERATOR_LABEL(HISTOGRAM, label_histogram())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_channel()
{
  static const List<String> data = List<String>() <<
    "Default" <<
    "Red" <<
    "Green" <<
    "Blue" <<
    "Alpha";
  
  DJV_ASSERT(data.size() == _CHANNEL_SIZE);

  return data;
}

const List<String> & label_filter()
{
  static const List<String> data = List<String>() <<
    "Nearest" <<
    "Linear" <<
    "Box" <<
    "Triangle" <<
    "Bell" <<
    "B-Spline" <<
    "Lanczos3" <<
    "Cubic" <<
    "Mitchell";
  
  DJV_ASSERT(data.size() == _FILTER_SIZE);
  
  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, const Xform & in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, const Color & in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, const Levels & in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, CHANNEL in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, const Filter & in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, FILTER in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, HISTOGRAM in)
{
  return debug << string::label(in);
}

//------------------------------------------------------------------------------
// Global
//------------------------------------------------------------------------------

Global::Global() :
  _filter(FILTER_LINEAR, FILTER_NEAREST)
{
  DJV_DEBUG("Global::Global");
}

Global::~Global()
{
  DJV_DEBUG("Global::~Global");
}

void Global::filter(const Filter & in)
{
  _filter = in;
}

const Filter & Global::filter() const
{
  return _filter;
}

//------------------------------------------------------------------------------

}
}

