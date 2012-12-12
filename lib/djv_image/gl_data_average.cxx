/*------------------------------------------------------------------------------
 lib/djv_image/gl_data_average.cxx

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
// average()
//------------------------------------------------------------------------------

void average(
  const data::Data & in,
  color::Color * out
) throw (Error) {

  DJV_ASSERT(out);

  DJV_DEBUG("average");
  DJV_DEBUG_PRINT("in = " << in);

  out->pixel(in.pixel());
  
  const data::Data * data = &in;
  data::Data tmp;
  data::Info info(in.size(), in.pixel());
  if (in.info() != info)
  {
    DJV_DEBUG_PRINT("copy");
    copy(in, &tmp);
    data = &tmp;
  }
  
  const int w = info.size.x, h = info.size.y;
  const double area = w * h;
  const int channels = pixel::channels(info.pixel);
  switch (pixel::type(info.pixel))
  {

#define _WORK(TYPE) \
  \
  const pixel::TYPE * p = reinterpret_cast< \
    const pixel::TYPE *>(data->data()); \
  double accum [pixel::channels_max]; \
  for (int c = 0; c < channels; ++c) accum[c] = 0.0; \
  for (int y = 0; y < h; ++y) \
    for (int x = 0; x < w; ++x, p += channels) \
      for (int c = 0; c < channels; ++c) \
        accum[c] += p[c] / area;

    case pixel::U8:
    {
      _WORK(U8_T)
      for (int c = 0; c < channels; ++c)
        out->set_u8(int(accum[c]), c);
    }
    break;
    
    case pixel::U16:
    {
      _WORK(U16_T)
      for (int c = 0; c < channels; ++c)
        out->set_u16(int(accum[c]), c);
    }
    break;
    
#if defined (DJV_OPENEXR)

    case pixel::F16:
    {
      _WORK(F16_T)
      for (int c = 0; c < channels; ++c)
        out->set_f16(static_cast<pixel::F16_T>(accum[c]), c);
    }
    break;

#endif

    case pixel::F32:
    {
      _WORK(F32_T)
      for (int c = 0; c < channels; ++c)
        out->set_f32(static_cast<pixel::F32_T>(accum[c]), c);
    }
    break;

    case pixel::U10:
    {
      const pixel::U10_S * p = reinterpret_cast<
        const pixel::U10_S *>(data->data());
      double accum [pixel::channels_max];
      for (int c = 0; c < channels; ++c) accum[c] = 0.0;
      for (int y = 0; y < h; ++y)
        for (int x = 0; x < w; ++x, ++p)
        {
          accum[0] += p->r / area;
          accum[1] += p->g / area;
          accum[2] += p->b / area;
        }
      for (int c = 0; c < channels; ++c)
        out->set_u10(int(accum[c]), c);
    }
    break;
    
    default: break;
  }
  
  DJV_DEBUG_PRINT("out = " << *out);
}

//------------------------------------------------------------------------------

}
}

