/*------------------------------------------------------------------------------
 lib/djv_image/io_sgi_data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_sgi.h"

namespace djv_image {
namespace io_sgi {

//------------------------------------------------------------------------------
// rle_load()
//------------------------------------------------------------------------------

namespace {

template<typename T>
bool load(
  const void * in,
  const void * end,
  void * out,
  int size,
  bool endian
) {
  DJV_DEBUG("load");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("endian = " << endian);
  
  const size_t bytes = sizeof(T);
  DJV_DEBUG_PRINT("bytes = " << bytes);

  const T * in_p = reinterpret_cast<const T *>(in);
  T * out_p = reinterpret_cast<T *>(out);
  const T * const out_end = out_p + size;

  while (out_p < out_end)
  {
    // Information.
    
    if (in_p > end) return false;
    
    const int count = *in_p & 0x7f;
    const bool run = ! (*in_p & 0x80);
    const int length = run ? 1 : count;
    DJV_DEBUG_PRINT("count = " << count);
    DJV_DEBUG_PRINT("  run = " << run);
    DJV_DEBUG_PRINT("  length = " << length);
    
    ++in_p;

    // Unpack.

    if (in_p + length > end) return false;
    
    if (run)
    {
      if (! endian)
      {
        for (int j = 0; j < count; ++j, ++out_p)
          *out_p = *in_p;
      }
      else
      {
        memory::endian(in_p, out_p, 1, bytes);
        if (count > 1)
          memory::set<T>(*out_p, out_p + 1, count - 1);
        out_p += count;
      }
      ++in_p;
    }
    else
    {
      if (! endian)
      {
        for (int j = 0; j < length; ++j, ++in_p, ++out_p)
          *out_p = *in_p;
      }
      else
      {
        memory::endian(in_p, out_p, length, bytes);
        in_p += length;
        out_p += length;
      }
    }
  }
  
  return true;
}

}

bool rle_load(
  const void * in,
  const void * end,
  void * out,
  int size,
  int bytes,
  bool endian
) {
  switch (bytes)
  {
    case 1: return load<uint8_t>(in, end, out, size, false);
    case 2: return load<uint16_t>(in, end, out, size, endian);
  }
  return false;
}

//------------------------------------------------------------------------------
// rle_save()
//------------------------------------------------------------------------------

namespace {

template<typename T>
size_t save(
  const void * in,
  void * out,
  int size,
  bool endian
) {
  DJV_DEBUG("save");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("endian = " << endian);
  
  const size_t bytes = sizeof(T);
  DJV_DEBUG_PRINT("bytes = " << bytes);

  const T * in_p = reinterpret_cast<const T *>(in);
  T * out_p = reinterpret_cast<T *>(out);
  const T * const end = in_p + size;
  
  while (in_p < end)
  {
    // Pixel runs.
    
    const int min = 3;
    const int max = math::min(0x7f, static_cast<int>(end - in_p));
    int count = 1, match = 1;
    for (; count < max; ++count)
    {
      if (in_p[count] == in_p[count - 1])
      {
        ++match;
        if (min == match && count >= min)
        {
          count -= min - 1;
          match = 1;
          break;
        }
      }
      else
      {
        if (match >= min) break;
        match = 1;
      }
    }
    const bool run = match > min;
    const int length = run ? 1 : count;
    DJV_DEBUG_PRINT("count = " << count);
    DJV_DEBUG_PRINT("  run = " << run);
    DJV_DEBUG_PRINT("  length = " << length);
    
    // Information.
    
    *out_p++ = (count & 0x7f) | ((! run) << 7);
    
    // Pack.

    if (! endian)
    {
      for (int i = 0; i < length; ++i)
        out_p[i] = in_p[i];
    }
    else
    {
      memory::endian(in_p, out_p, length, bytes);
    }
    
    out_p += length;
    in_p += count;
  }
  
  // Cap the end.
  
  *out_p++ = 0;
  
  const size_t r = (out_p - reinterpret_cast<T *>(out)) * bytes;
  DJV_DEBUG_PRINT("r = " << r);
  return r;
}

}

size_t rle_save(
  const void * in,
  void * out,
  int size,
  int bytes,
  bool endian
) {
  switch (bytes)
  {
    case 1: return save<uint8_t>(in, out, size, false);
    case 2: return save<uint16_t>(in, out, size, endian);
  }
  return 0;
}

//------------------------------------------------------------------------------

}
}

