/*------------------------------------------------------------------------------
 lib/djv_image/gl-2.0_data_scale.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "gl-2.0_data_private.h"

namespace djv_image {
namespace gl_data {

//------------------------------------------------------------------------------
// Filters
//------------------------------------------------------------------------------

namespace {

typedef double (Filter_Fnc)(const double t);

static const double support_box = 0.5;

static double filter_box(double t)
{
  if (t > -0.5 && t <= 0.5) return 1.0;
  return 0.0;
}

static const double support_triangle = 1.0;

static double filter_triangle(double t)
{
  if (t < 0.0) t = -t;
  if (t < 1.0) return 1.0 - t;
  return 0.0;
}

static const double support_bell = 1.5;

static double filter_bell(double t)
{
  if (t < 0.0) t = -t;
  if (t < 0.5) return 0.75 - t * t;
  if (t < 1.5)
  {
    t = t - 1.5;
    return 0.5 * t * t;
  }
  return 0.0;
}

static const double support_bspline = 2.0;

static double filter_bspline(double t)
{
  if (t < 0.0) t = -t;
  if (t < 1.0)
  {
    const double tt = t * t;
    return (0.5 * tt * t) - tt + 2.0 / 3.0;
  }
  else if (t < 2.0)
  {
    t = 2.0 - t;
    return (1.0 / 6.0) * (t * t * t);
  }
  return 0.0;
}

static double sinc(double x)
{
  x *= math::pi;
  if (x != 0.0) return math::sin(x) / x;
  return 1.0;
}

static const double support_lanczos3 = 3.0;

static double filter_lanczos3(double t)
{
  if (t < 0.0) t = -t;
  if (t < 3.0) return sinc(t) * sinc(t / 3.0);
  return 0.0;
}

static const double support_cubic = 1.0;

static double filter_cubic(double t)
{
  if (t < 0.0) t = -t;
  if (t < 1.0) return (2.0 * t - 3.0) * t * t + 1.0;
  return 0.0;
}

static const double support_mitchell = 2.0;

static double filter_mitchell(double t)
{
  const double tt = t * t;
  static const double b = 1.0 / 3.0;
  static const double c = 1.0 / 3.0;
  if (t < 0.0) t = -t;
  if (t < 1.0)
  {
    t =
      ((12.0 - 9.0 * b - 6.0 * c) * (t * tt)) +
      ((-18.0 + 12.0 * b + 6.0 * c) * tt) +
      (6.0 - 2.0 * b);
    return t / 6.0;
  }
  else if (t < 2.0)
  {
    t =
      ((-1.0 * b - 6.0 * c) * (t * tt)) +
      ((6.0 * b + 30.0 * c) * tt) +
      ((-12.0 * b - 48.0 * c) * t) +
      (8.0 * b + 24.0 * c);
    return t / 6.0;
  }
  return 0.0;
}

Filter_Fnc * filter_fnc(FILTER in)
{
  static Filter_Fnc * tmp [] =
  {
    filter_box,
    filter_box,
    filter_box,
    filter_triangle,
    filter_bell,
    filter_bspline,
    filter_lanczos3,
    filter_cubic,
    filter_mitchell
  };
  return tmp[in];
}

static double filter_support(FILTER in)
{
  static const double tmp [] =
  {
    support_box,
    support_box,
    support_box,
    support_triangle,
    support_bell,
    support_bspline,
    support_lanczos3,
    support_cubic,
    support_mitchell
  };
  return tmp[in];
}

}

//------------------------------------------------------------------------------
// scale_contrib()
//------------------------------------------------------------------------------

namespace {

int edge(int in, int size)
{
  return math::clamp(in, 0, size - 1);
}

}

void scale_contrib(
  int input,
  int output,
  FILTER filter,
  data::Data * out
) {
  DJV_DEBUG("scale_contrib");
  DJV_DEBUG_PRINT("scale = " << input << " " << output);
  DJV_DEBUG_PRINT("filter = " << filter);

  // Filter function.

  Filter_Fnc * fnc = filter_fnc(filter);
  const double support = filter_support(filter);
  
  DJV_DEBUG_PRINT("support = " << support);

  const double scale =
    static_cast<double>(output) / static_cast<double>(input);

  DJV_DEBUG_PRINT("scale = " << scale);

  const double radius =
    support * (scale >= 1.0 ? 1.0 : (1.0 / scale));

  DJV_DEBUG_PRINT("radius = " << radius);

  // Initialize.

  const int width = math::ceil(radius * 2 + 1);

  //DJV_DEBUG_PRINT("width = " << width);
  
  out->set(data::Info(V2i(output, width), pixel::LA_F32));

  // Work.
  
  for (int i = 0; i < output; ++i)
  {
    const double center = i / scale;
    const int left = math::ceil(center - radius);
    const int right = math::floor(center + radius);
    
    //DJV_DEBUG_PRINT(i << " = " << left << " " << center << " " << right);
    
    double sum = 0.0;
    int pixel = 0;
    int j = 0;
    for (int k = left; j < width && k <= right; ++j, ++k)
    {
      pixel::F32_T * p = reinterpret_cast<pixel::F32_T *>(out->data(i, j));

      pixel = edge(k, input);
      
      const double x = (center - k) * (scale < 1.0 ? scale : 1.0);
      const double w = (scale < 1.0) ? ((*fnc)(x) * scale) : (*fnc)(x);
      
      //DJV_DEBUG_PRINT("w = " << w);
      
      p[0] = static_cast<pixel::F32_T>(pixel);
      p[1] = static_cast<pixel::F32_T>(w);
      
      sum += w;
    }
    for (; j < width; ++j)
    {
      pixel::F32_T * p = reinterpret_cast<pixel::F32_T *>(out->data(i, j));
      
      p[0] = static_cast<pixel::F32_T>(pixel);
      p[1] = 0.0f;
    }
    
    /*for (j = 0; j < width; ++j)
    {
      pixel::F32_T * p = reinterpret_cast<pixel::F32_T *>(out->data(i, j));

      DJV_DEBUG_PRINT(p[0] << " = " << p[1]);
    }
    DJV_DEBUG_PRINT("sum = " << sum);*/

    // XXX?
    
    for (j = 0; j < width; ++j)
    {
      pixel::F32_T * p = reinterpret_cast<pixel::F32_T *>(out->data(i, j));

      p[1] /= static_cast<pixel::F32_T>(sum);
    }
  }
}

//------------------------------------------------------------------------------

}
}

