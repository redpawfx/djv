/*------------------------------------------------------------------------------
 lib/djv_image/io_ppm_data.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_ppm.h"

namespace djv_image {
namespace io_ppm {

//------------------------------------------------------------------------------
// bytes_scanline()
//------------------------------------------------------------------------------

size_t bytes_scanline(int width, int channels, int bit_depth, DATA data)
{
  DJV_DEBUG("bytes_scanline");
  DJV_DEBUG_PRINT("width = " << width);
  DJV_DEBUG_PRINT("channels = " << channels);
  DJV_DEBUG_PRINT("bit depth = " << bit_depth);
  DJV_DEBUG_PRINT("data = " << data);
  
  size_t out = 0;
  
  switch (data)
  {
    case DATA_ASCII:
    {
      int chars = 0;
      switch (bit_depth)
      {
        case  1: chars = 1; break;
        case  8: chars = 3; break;
        case 16: chars = 5; break;
        default: break;
      }
      out = (chars + 1) * width * channels + 1;
    }
    break;
    
    case DATA_BINARY:
      switch (bit_depth)
      {
        case  1: out = math::ceil(width / 8.0); break;
        case  8:
        case 16: out = width * channels; break;
        default: break;
      }
      break;
  }
  
  DJV_DEBUG_PRINT("out = " << static_cast<int>(out));
  
  return out;
}

//------------------------------------------------------------------------------
// ascii_load()
//------------------------------------------------------------------------------

void ascii_load(File_Io & io, void * out, int size, int bit_depth) throw (Error)
{
  DJV_DEBUG("ascii_load");
  
  char tmp [string::cstring_size] = "";
  int i = 0;
  
  switch (bit_depth)
  {
    case 1:
    {
      uint8_t * out_p = reinterpret_cast<uint8_t *>(out);
      for (; i < size; ++i)
      {
        file_io::word(io, tmp, string::cstring_size);
        out_p[i] = string::string_to_int(tmp) ? 0 : 255;
      }
    }
    break;
    
#define _LOAD(TYPE) \
      \
      TYPE * out_p = reinterpret_cast<TYPE *>(out); \
      for (; i < size; ++i) \
      { \
        file_io::word(io, tmp, string::cstring_size); \
        out_p[i] = string::string_to_int(tmp); \
        DJV_DEBUG_PRINT(i << " = " << out_p[i]); \
      }

    case 8:  { _LOAD(uint8_t) } break;
    case 16: { _LOAD(uint16_t) } break;
    
    default: break;
  }
}

//------------------------------------------------------------------------------
// ascii_save()
//------------------------------------------------------------------------------

size_t ascii_save(const void * in, void * out, int size, int bit_depth)
{
  char * out_p = reinterpret_cast<char *>(out);
  
  switch (bit_depth)
  {
    case 1:
    {
      const uint8_t * in_p = reinterpret_cast<const uint8_t *>(in);
      for (int i = 0; i < size; ++i)
      {
        out_p[0] = '0' + (! in_p[i]);
        out_p[1] = ' ';
        out_p += 2;
      }
    }
    break;
    
#define _SAVE(TYPE) \
      \
      const TYPE * in_p = reinterpret_cast<const TYPE *>(in); \
      for (int i = 0; i < size; ++i) \
      { \
        const String::size_type j = string::int_to_string(in_p[i], out_p); \
        out_p[j] = ' '; \
        out_p += j + 1; \
      }

    case 8:  { _SAVE(uint8_t) } break;
    case 16: { _SAVE(uint16_t) } break;
    
    default: break;
  }
  
  *out_p++ = '\n';
  
  return out_p - reinterpret_cast<char *>(out);
}

//------------------------------------------------------------------------------

}
}
