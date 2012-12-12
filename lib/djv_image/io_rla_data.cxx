/*------------------------------------------------------------------------------
 lib/djv_image/io_rla_data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_rla.h"

namespace djv_image {
namespace io_rla {

//------------------------------------------------------------------------------
// rle_load()
//------------------------------------------------------------------------------

void rle_load(
  File_Io & io,
  uint8_t * out,
  int size,
  int channels,
  int bytes
) throw (Error) {
  DJV_DEBUG("rle_load");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("channels = " << channels);
  DJV_DEBUG_PRINT("bytes = " << bytes);
  
  int16_t _size = 0;
  io.get_16(&_size);
  DJV_DEBUG_PRINT("io size = " << _size);
  
  const uint8_t * start = io.mmap_p();
  const uint8_t * p = start;
  io.seek(_size);
  
  for (int b = 0; b < bytes; ++b)
  {
    uint8_t * out_p = out + (memory::LSB == memory::endian() ? (bytes - 1 - b) : b);
    const int out_inc = channels * bytes;
    
    for (int i = 0; i < size;)
    {
      int count = *((int8_t *)p);
      ++p;
      DJV_DEBUG_PRINT("count = " << count);

      if (count >= 0)
      {
        ++count;
        for (int j = 0; j < count; ++j, out_p += out_inc)
          *out_p = *p;
        ++p;
      }
      else
      {
        count = -count;
        for (int j = 0; j < count; ++j, ++p, out_p += out_inc)
          *out_p = *p;
      }
    
      i += count;
    }
  }
  
  DJV_DEBUG_PRINT("out = " << p - start);
}

//------------------------------------------------------------------------------
// float_load()
//------------------------------------------------------------------------------

void float_load(
  File_Io & io,
  uint8_t * out,
  int size,
  int channels
) throw (Error) {

  DJV_DEBUG("float_load");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("channels = " << channels);
  
  int16_t _size = 0;
  io.get_16(&_size);
  DJV_DEBUG_PRINT("io size = " << _size);
  
  const uint8_t * start = io.mmap_p();
  const uint8_t * p = start;
  io.seek(_size);
  
  const int out_inc = channels * 4;
  if (memory::LSB == memory::endian())
    for (int i = 0; i < size; ++i, p += 4, out += out_inc)
    {
      out[0] = p[3];
      out[1] = p[2];
      out[2] = p[1];
      out[3] = p[0];
    }
  else
    for (int i = 0; i < size; ++i, p += 4, out += out_inc)
    {
      out[0] = p[0];
      out[1] = p[1];
      out[2] = p[2];
      out[3] = p[3];
    }

  DJV_DEBUG_PRINT("out = " << p - start);
}

//------------------------------------------------------------------------------
// skip()
//------------------------------------------------------------------------------

void skip(File_Io & io) throw (Error)
{
  int16_t size = 0;
  io.get_16(&size);
  io.seek(size);
}

//------------------------------------------------------------------------------

}
}
