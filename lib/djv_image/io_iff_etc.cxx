/*------------------------------------------------------------------------------
 lib/djv_image/io_iff_etc.cxx

 Copyright (c) 2008-2009 Mikael Sundell
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_iff.h"

namespace djv_image {
namespace io_iff {

//------------------------------------------------------------------------------
// Align, Tile size
//------------------------------------------------------------------------------

uint32_t align_size (uint32_t size, uint32_t alignment)
{
  uint32_t mod = size % alignment;
  if (mod)
  {
    mod = alignment - mod;
    size += mod;
  }
  return size;
}

V2i tile_size (uint32_t width, uint32_t height)
{
  uint32_t tw = tile_width(), th = tile_height();
  V2i size = V2i((width + tw - 1) / tw,
                 (height + th - 1) / th);
  return (size);
}

const int & tile_width()
{
  static int tile_w = 64;
  return tile_w;
}

const int & tile_height()
{
  static int tile_h = 64;
  return tile_h;
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(COMPRESSION, label_compression())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_compression()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "RLE";

  DJV_ASSERT(data.size() == _COMPRESSION_SIZE);

  return data;
}

const List<String> & label_compression_text()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "RLE (Run Length Encoded)";
  
  DJV_ASSERT(data.size() == _COMPRESSION_SIZE);

  return data;
}

const List<String> & label_save_option()
{
  static const List<String> data = List<String>() <<
    "Compression";

  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);

  return data;
}

//------------------------------------------------------------------------------

}
}

