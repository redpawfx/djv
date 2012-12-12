/*------------------------------------------------------------------------------
 lib/djv_image/gl_data_histogram.cxx

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
// histogram(), histogram_size()
//------------------------------------------------------------------------------

namespace {

int shift_10(HISTOGRAM in)
{
  switch (in)
  {
    case HISTOGRAM_256: return 2;
    case HISTOGRAM_1024: return 0;
  }
  return 0;
}

int shift_16(HISTOGRAM in)
{
  switch (in)
  {
    case HISTOGRAM_256: return 8;
    case HISTOGRAM_1024: return 6;
  }
  return 0;
}

}

void histogram(
  const data::Data & in,
  data::Data * out,
  HISTOGRAM histogram,
  color::Color * min,
  color::Color * max
) throw (Error) {

  DJV_ASSERT(out);

  DJV_DEBUG("histogram");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("histogram = " << histogram_size(histogram));
  
  const int size = histogram_size(histogram);
  out->set(Info(V2i(size, 1), pixel::RGB_U16));
  out->zero();
  
  DJV_DEBUG_PRINT("out = " << *out);
  
  const data::Data * data = &in;
  data::Data tmp;
  data::Info info(in.size(), in.pixel());
  if (in.info() != info)
  {
    copy(in, &tmp);
    data = &tmp;
  }

  *min = color::Color(info.pixel);
  *max = color::Color(info.pixel);
  
  const int w = info.size.x, h = info.size.y;
  const int channels = pixel::channels(info.pixel);
  switch (info.pixel)
  {
    case pixel::RGB_U10:
    {
      const pixel::U10_S * in_p =
        reinterpret_cast<const pixel::U10_S *>(data->data());
      const pixel::U10_S * const end = in_p + w * h;
      pixel::U16_T * const out_p =
        reinterpret_cast<pixel::U16_T *>(out->data());
      pixel::U10_S * const min_p =
        reinterpret_cast<pixel::U10_S *>(min->data());
      pixel::U10_S * const max_p =
        reinterpret_cast<pixel::U10_S *>(max->data());

      if (w && h)
      {
        min_p->r = max_p->r = in_p->r;
        min_p->g = max_p->g = in_p->g;
        min_p->b = max_p->b = in_p->b;
      }

      const int shift = shift_10(histogram);
      for (; in_p < end; ++in_p)
      {
        out_p[(in_p->r >> shift) * 3 + 0]++;
        out_p[(in_p->g >> shift) * 3 + 1]++;
        out_p[(in_p->b >> shift) * 3 + 2]++;
          
        min_p->r = math::min(in_p->r, min_p->r);
        min_p->g = math::min(in_p->g, min_p->g);
        min_p->b = math::min(in_p->b, min_p->b);
        max_p->r = math::max(in_p->r, max_p->r);
        max_p->g = math::max(in_p->g, max_p->g);
        max_p->b = math::max(in_p->b, max_p->b);
      }

    }
    break;

#define _WORK(TYPE) \
  \
  const pixel::TYPE##_T * in_p = \
    reinterpret_cast<const pixel::TYPE##_T *>(data->data()); \
  const pixel::TYPE##_T * const end = in_p + w * h * channels; \
  pixel::U16_T * const out_p = \
    reinterpret_cast<pixel::U16_T *>(out->data()); \
  pixel::TYPE##_T * const min_p = \
    reinterpret_cast<pixel::TYPE##_T *>(min->data()); \
  pixel::TYPE##_T * const max_p = \
    reinterpret_cast<pixel::TYPE##_T *>(max->data()); \
  \
  if (w && h) \
    for (int c = 0; c < channels; ++c) \
      min_p[c] = max_p[c] = in_p[c]; \
  \
  const int shift = shift_16(histogram); \
  for (; in_p < end; in_p += channels) \
    switch (channels) \
    { \
      case 4: \
        min_p[3] = math::min(in_p[3], min_p[3]); \
        max_p[3] = math::max(in_p[3], max_p[3]); \
      case 3: \
        out_p[(PIXEL_##TYPE##_TO_U16(in_p[2]) >> shift) * 3 + 2]++; \
        min_p[2] = math::min(in_p[2], min_p[2]); \
        max_p[2] = math::max(in_p[2], max_p[2]); \
      case 2: \
        out_p[(PIXEL_##TYPE##_TO_U16(in_p[1]) >> shift) * 3 + 1]++; \
        min_p[1] = math::min(in_p[1], min_p[1]); \
        max_p[1] = math::max(in_p[1], max_p[1]); \
      case 0: \
        out_p[(PIXEL_##TYPE##_TO_U16(in_p[0]) >> shift) * 3 + 0]++; \
        min_p[0] = math::min(in_p[0], min_p[0]); \
        max_p[0] = math::max(in_p[0], max_p[0]); \
        break; \
    }

    case pixel::L_U8:
    case pixel::LA_U8:
    case pixel::RGB_U8:
    case pixel::RGBA_U8: { _WORK(U8); } break;

    case pixel::L_U16:
    case pixel::LA_U16:
    case pixel::RGB_U16:
    case pixel::RGBA_U16: { _WORK(U16); } break;

#if defined (DJV_OPENEXR)
    case pixel::L_F16:
    case pixel::LA_F16:
    case pixel::RGB_F16:
    case pixel::RGBA_F16: { _WORK(F16); } break;
#endif

    case pixel::L_F32:
    case pixel::LA_F32:
    case pixel::RGB_F32:
    case pixel::RGBA_F32: { _WORK(F32); } break;
    
    default: break;
  }
  
/*#if defined(DJV_DEBUG)
  pixel::U16_T * const out_p =
    reinterpret_cast<pixel::U16_T *>(out->data());
  for (int i = 0; i < size; ++i)
    DJV_DEBUG_PRINT(i << " = " << out_p[i * 3]);
#endif*/
}

int histogram_size(HISTOGRAM in)
{
  static const int data [] =
  {
    256,
    1024
  };

  return data[in];
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_histogram()
{
  static const List<String> data = List<String>() <<
    "256" <<
    "1024";

  return data;
}

//------------------------------------------------------------------------------

}
}

