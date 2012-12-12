/*------------------------------------------------------------------------------
 lib/djv_image/io_pic_data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_pic.h"

namespace djv_image {
namespace io_pic {

//------------------------------------------------------------------------------
// rle_load()
//------------------------------------------------------------------------------

const uint8_t * rle_load(
  const uint8_t * in,
  const uint8_t * end,
  uint8_t * out,
  int size,
  int channels,
  int stride,
  bool endian
) {
  DJV_DEBUG("rle_load");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("channels = " << channels);
  DJV_DEBUG_PRINT("stride = " << stride);

  const uint8_t * const out_end = out + size * stride;
  while (in < end && out < out_end)
  {
    // Get RLE information.

    uint16_t count = *in++;
    //DJV_DEBUG_PRINT("count = " << count);
    if (count >= 128)
    {
      if (128 == count)
      {
        if (endian) memory::endian(in, &count, 1, 2);
        else memory::copy(in, &count, 2);
        in += 2;
      }
      else
      {
        count -= 127;
      }
      //DJV_DEBUG_PRINT("repeat = " << count);

      const uint8_t * p = in;
      in += channels;
      if (in > end) break;

      for (uint16_t i = 0; i < count; ++i, out += stride)
        for (int j = 0; j < channels; ++j)
          out[j] = p[j];
    }
    else
    {
      ++count;
      //DJV_DEBUG_PRINT("raw = " << count);
    
      const uint8_t * p = in;
      in += count * channels;
      if (in > end) break;
      
      for (uint16_t i = 0; i < count; ++i, p += channels, out += stride)
        for (int j = 0; j < channels; ++j)
          out[j] = p[j];
    }
  }
  
  return in > end ? 0 : in;
}

//------------------------------------------------------------------------------

}
}
