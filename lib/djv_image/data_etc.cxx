/*------------------------------------------------------------------------------
 lib/djv_image/data_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "data.h"

#include <djv_base/box.h>

namespace djv_image {
namespace data {

//------------------------------------------------------------------------------
// bytes_scanline(), bytes_data()
//------------------------------------------------------------------------------

size_t bytes_scanline(const Info & in)
{
  return (in.size.x * pixel::bytes(in.pixel) * in.align) / in.align;
}

size_t bytes_data(const Info & in)
{
  return in.size.y * bytes_scanline(in);
}

//------------------------------------------------------------------------------
// proxy_scale()
//------------------------------------------------------------------------------

void proxy_scale(const Data & in, Data * out, PROXY proxy)
{
  DJV_ASSERT(out);
  
  DJV_DEBUG("proxy_scale");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("out = " << *out);
  DJV_DEBUG_PRINT("proxy = " << proxy);
  
  const int w = out->w();
  const int h = out->h();
  const int proxy_scale = data::proxy_scale(proxy);
  const bool bgr = in.info().bgr != out->info().bgr;
  const bool endian = in.info().endian != memory::endian();
  
  const bool fast = in.pixel() == out->pixel() && ! bgr;
  
  memory::Buffer<uint8_t> tmp;
  if (! fast && endian)
    tmp.size(w * proxy_scale * bytes(in.pixel()));
  
  for (int y = 0; y < h; ++y)
  {
    const uint8_t * in_p = in.data(0, y * proxy_scale);
    uint8_t * out_p = out->data(0, y);
        
    if (fast)
    {
      const size_t bytes_pixel = in.bytes_pixel();
      const size_t in_stride = bytes_pixel * proxy_scale;
      const size_t out_stride = bytes_pixel;

      for (
        int x = 0;
        x < w;
        ++x, in_p += in_stride, out_p += out_stride
      )
        switch (bytes_pixel)
        {
          case 16: out_p[15] = in_p[15];
          case 15: out_p[14] = in_p[14];
          case 14: out_p[13] = in_p[13];
          case 13: out_p[12] = in_p[12];
          case 12: out_p[11] = in_p[11];
          case 11: out_p[10] = in_p[10];
          case 10: out_p[9] = in_p[9];
          case 9: out_p[8] = in_p[8];
          case 8: out_p[7] = in_p[7];
          case 7: out_p[6] = in_p[6];
          case 6: out_p[5] = in_p[5];
          case 5: out_p[4] = in_p[4];
          case 4: out_p[3] = in_p[3];
          case 3: out_p[2] = in_p[2];
          case 2: out_p[1] = in_p[1];
          case 1: out_p[0] = in_p[0];
        }
    }
    else
    {
      if (endian)
      {
        memory::endian(in_p, tmp(), w * proxy_scale, bytes(in.pixel()));
        in_p = tmp();
      }
    
      pixel::convert(
        in_p, in.pixel(),
        out_p, out->pixel(),
        w,
        proxy_scale,
        bgr
      );
    }
  }
}

int proxy_scale(PROXY proxy)
{
  return proxy ? math::pow(2, static_cast<int>(proxy)) : 1;
}

V2i proxy_scale(const V2i & in, PROXY proxy)
{
  const int scale = proxy_scale(proxy);
  
  return V2i(
    math::ceil(in.x / static_cast<double>(scale)),
    math::ceil(in.y / static_cast<double>(scale))
  );
}

Box2i proxy_scale(const Box2i & in, PROXY proxy)
{
  const int scale = proxy_scale(proxy);
  
  return Box2i(
    math::ceil(in.x / static_cast<double>(scale)),
    math::ceil(in.y / static_cast<double>(scale)),
    math::ceil(in.size.x / static_cast<double>(scale)),
    math::ceil(in.size.y / static_cast<double>(scale))
  );
}

//------------------------------------------------------------------------------
// planar_interleave()
//------------------------------------------------------------------------------

void planar_interleave(const Data & in, Data * out, PROXY proxy)
{
  DJV_ASSERT(out);
  DJV_ASSERT(in.pixel() == out->pixel());
  DJV_ASSERT(in.pixel() != pixel::RGB_U10);
  
  DJV_DEBUG("planar_interleave");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("out = " << *out);
  DJV_DEBUG_PRINT("proxy = " << proxy);
  
  const int w = out->w(), h = out->h();
  const int proxy_scale = data::proxy_scale(proxy);
  const int channels = out->channels();
  const size_t bytes_pixel = out->bytes_pixel();
  const int channel_bytes = pixel::channel_bytes(out->pixel());
  
  for (int c = 0; c < channels; ++c)
    for (int y = 0; y < h; ++y)
      /*switch (channel_bytes)
      {
        default:*/
        {
          const uint8_t * in_p = in.data() + (c * in.h() + y * proxy_scale) *
            in.w() * channel_bytes;
          uint8_t * out_p = out->data(0, y) + c * channel_bytes;
          for (
            int x = 0;
            x < w;
            ++x, in_p += channel_bytes * proxy_scale, out_p += bytes_pixel
          )
            switch (channel_bytes)
            {
              case 4: out_p[3] = in_p[3];
              case 3: out_p[2] = in_p[2];
              case 2: out_p[1] = in_p[1];
              case 1: out_p[0] = in_p[0];
            }
        //}
        //break;
      }
}

//------------------------------------------------------------------------------
// planar_deinterleave()
//------------------------------------------------------------------------------

void planar_deinterleave(const Data & in, Data * out)
{
  DJV_ASSERT(out);
  DJV_ASSERT(in.pixel() == out->pixel());
  DJV_ASSERT(in.pixel() != pixel::RGB_U10);
  
  DJV_DEBUG("planar_deinterleave");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("out = " << *out);
  
  const int w = out->w(), h = out->h();
  const int channels = out->channels();
  const size_t bytes_pixel = out->bytes_pixel();
  const int channel_bytes = pixel::channel_bytes(out->pixel());
  
  for (int c = 0; c < channels; ++c)
    for (int y = 0; y < h; ++y)
      /*switch (channel_bytes)
      {
        default:*/
        {
          const uint8_t * in_p = in.data(0, y) + c * channel_bytes;
          uint8_t * out_p = out->data() + (c * h + y) * w * channel_bytes;
          for (
            int x = 0;
            x < w;
            ++x, in_p += bytes_pixel, out_p += channel_bytes
          )
            switch (channel_bytes)
            {
              case 4: out_p[3] = in_p[3];
              case 3: out_p[2] = in_p[2];
              case 2: out_p[1] = in_p[1];
              case 1: out_p[0] = in_p[0];
            }
        //}
        //break;
      }
}

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool operator == (const Info & a, const Info & b)
{
  return
    a.size == b.size &&
    a.proxy == b.proxy &&
    a.pixel == b.pixel &&
    a.bgr == b.bgr &&
    a.mirror == b.mirror &&
    a.align == b.align &&
    a.endian == b.endian;
}

bool operator != (const Info & a, const Info & b)
{
  return ! (a == b);
}

bool operator == (const Data & a, const Data & b)
{
  return
    a.info() == b.info() &&
    a.bytes_data() == b.bytes_data() &&
    memory::compare(a.data(), b.data(), a.bytes_data());
}

bool operator != (const Data & a, const Data & b)
{
  return ! (a == b);
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(PROXY, label_proxy())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_proxy()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "1/2" <<
    "1/4" <<
    "1/8";
    
  DJV_ASSERT(data.size() == _PROXY_SIZE);

  return data;
}

//------------------------------------------------------------------------------
// Debugging
//------------------------------------------------------------------------------

Debug & operator << (Debug & debug, PROXY in)
{
  return debug << string::label(in);
}

Debug & operator << (Debug & debug, const Info & in)
{
  return debug <<
    //"file name: " << in.file_name << ", " <<
    //"layer name: " << in.layer_name << ", " <<
    "size: " << in.size << ", " <<
    "proxy: " << in.proxy << ", " <<
    "pixel: " << in.pixel << ", " <<
    "bgr: " << in.bgr << ", " <<
    "mirror: " << in.mirror << ", " <<
    "align: " << in.align << ", " <<
    "endian: " << in.endian;
}

Debug & operator << (Debug & debug, const Data & in)
{
  return debug << in.info();
}

//------------------------------------------------------------------------------

}
}

