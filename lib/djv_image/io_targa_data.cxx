/*------------------------------------------------------------------------------
 lib/djv_image/io_targa_data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_targa.h"

namespace djv_image {
namespace io_targa {

//------------------------------------------------------------------------------
// rle_load()
//------------------------------------------------------------------------------

const uint8_t * rle_load(
  const uint8_t * in,
  const uint8_t * end,
  uint8_t * out,
  int size,
  int channels
) {
  DJV_DEBUG("rle_load");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("channels = " << channels);

  const uint8_t * const out_end = out + size * channels;
  while (out < out_end)
  {
    // Information.

    if (in > end) return 0;

    const uint8_t count = (*in & 0x7f) + 1;
    const bool run = (*in & 0x80) ? true : false;
    const int length = run ? 1 : count;
    //DJV_DEBUG_PRINT("length = " << length);

    ++in;
    
    // Unpack.
    
    if (in + length * channels > end) return 0;

    if (run)
    {
      for (int j = 0; j < count; ++j, out += channels)
        switch (channels)
        {
          case 4: out[3] = in[3];
          case 3: out[2] = in[2];
          case 2: out[1] = in[1];
          case 1: out[0] = in[0]; break;
        }
      in += channels;
    }
    else
    {
      for (int j = 0; j < count; ++j, in += channels, out += channels)
        switch (channels)
        {
          case 4: out[3] = in[3];
          case 3: out[2] = in[2];
          case 2: out[1] = in[1];
          case 1: out[0] = in[0]; break;
        }
    }
  }
  
  return in;
}

//------------------------------------------------------------------------------
// rle_save()
//------------------------------------------------------------------------------

size_t rle_save(
  const uint8_t * in,
  uint8_t * out,
  int size,
  int channels
) {
  DJV_DEBUG("rle_save");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("channels = " << channels);

  const uint8_t * const _out = out;
  const uint8_t * const end = in + size * channels;
  while (in < end)
  {
    // Find runs.
    
    const int max = math::min(0x7f + 1, static_cast<int>(end - in) / channels);
    int count = 1;
    for (; count < max; ++count)
    {
      int c = 0;
      for (; c < channels; ++c)
        if (in[count * channels + c] != in[(count - 1) * channels + c]) break;
      if (c < channels) break;
    }
    const bool run = count > 1;
    const int length = run ? 1 : count;
    DJV_DEBUG_PRINT("count = " << count);
    DJV_DEBUG_PRINT("  run = " << run);
    DJV_DEBUG_PRINT("  length = " << length);
    
    // Information.
    
    *out++ = ((count - 1) & 0x7f) | (run << 7);
    
    // Pack.
    
    for (int j = 0; j < length * channels; j += channels)
      switch (channels)
      {
        case 4: out[j + 3] = in[j + 3];
        case 3: out[j + 2] = in[j + 2];
        case 2: out[j + 1] = in[j + 1];
        case 1: out[j    ] = in[j    ]; break;
      }
    
    out += length * channels;
    in += count * channels;
  }
  
  const size_t r = out - _out;
  DJV_DEBUG_PRINT("return = " << static_cast<int>(r));
  return r;
}

//------------------------------------------------------------------------------

}
}
