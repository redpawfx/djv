/*------------------------------------------------------------------------------
 lib/djv_image/io_iff_data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_iff.h"

namespace djv_image {
namespace io_iff {

//------------------------------------------------------------------------------
// rle_load()
//------------------------------------------------------------------------------

size_t
rle_load(
  const uint8_t * in,
  uint8_t * out,
  int size
)
{
  DJV_DEBUG("rle_load");
  DJV_DEBUG_PRINT("size = " << size);
  
  const uint8_t * const _in = in;
  const uint8_t * const end = out + size;

  while (out < end)
  {
    // Information.

    const uint8_t count = (*in & 0x7f) + 1;
    const bool run = (*in & 0x80) ? true : false;
    ++in;

    // Find runs.
    
    if (!run)
    {
      // Verbatim.

      for (int i=0; i<count; i++)
        *out++ = *in++;
    }
    else
    {
      // Duplicate.
      
      const uint8_t p = *in++;
      for (int i=0; i<count; i++)
        *out++ = p;
    }
  }
  const size_t r = in - _in;
  DJV_DEBUG_PRINT("tile r = " << static_cast<int>(r));  
  return r;
}


//------------------------------------------------------------------------------
// rle_save()
//------------------------------------------------------------------------------

namespace
{
  void
  save_verbatim (
    const uint8_t *& in,
    uint8_t *& out,
    int size)
  {
    int count = 1;
    unsigned char byte;

    // Two in a row or count.
    
    for (; count < size; ++count)
    {
      if (in[count - 1] == in[count]) 
      {
        if (byte == in[count - 1])
        {
          count -= 2;
          break;
        }
      }
      byte = in[count - 1];
    }

    // Information.
    
    *out++ = count - 1;
    memory::copy(in, out, count);
    
    out += count;
    in += count;
  }

  void
  save_duplicate (
    const uint8_t *& in,
    uint8_t *& out,
    int size)
  {
    int count = 1;
    for (; count < size; ++count)
    {
      if (in[count - 1] != in[count]) break; 
    }
    
    const bool run = count > 1;
    const int length = run ? 1 : count;
      
    // Information.

    *out++ = ((count - 1) & 0x7f) | (run << 7);
    *out = *in;

    out += length;
    in += count;
  }
}

size_t 
rle_save (
  const uint8_t * in,
  uint8_t * out,
  int size)
{
  DJV_DEBUG("rle_save");
  DJV_DEBUG_PRINT("size = " << size);

  const uint8_t * const _out = out;
  const uint8_t * const end = in + size;
  
  while (in < end) 
  {
    // Find runs.
    
    const int max = math::min(0x7f + 1, static_cast<int>(end - in));
    if (max > 0)
    {
      if (in[0] != in[1])
      {
        // Verbatim.

        save_verbatim (in, out, max);
      }
      else
      {
        // Duplicate.
        
        save_duplicate (in, out, max);
      }
    }
  }
  
  const size_t r = out - _out;
  DJV_DEBUG_PRINT("tile r = " << static_cast<int>(r));
  return r;
}

}
}
