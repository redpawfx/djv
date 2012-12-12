/*------------------------------------------------------------------------------
 lib/djv_image/io_targa_info.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_targa.h"

namespace djv_image {
namespace io_targa {

//------------------------------------------------------------------------------
// Header
//------------------------------------------------------------------------------

namespace {

struct Header
{
  Header();

  void load(File_Io &, io::Info &, bool * compression) throw (Error);
  void save(File_Io &, const io::Info &, bool compression) throw (Error);
  
  void debug() const;

private:

  struct Data
  {
    uint8_t  id_size;
    uint8_t  cmap_type;
    uint8_t  image_type;
    uint16_t cmap_start;
    uint16_t cmap_size;
    uint8_t  cmap_bits;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint8_t  pixel_bits;
    uint8_t  descriptor;
  } _data;
};

Header::Header()
{
  _data.id_size = 0;
  _data.cmap_type = 0;
  _data.image_type = 0;
  _data.cmap_start = 0;
  _data.cmap_size = 0;
  _data.cmap_bits = 0;
  _data.x = 0;
  _data.y = 0;
  _data.width = 0;
  _data.height = 0;
  _data.pixel_bits = 0;
  _data.descriptor = 0;
}

void Header::load(File_Io & io, io::Info & info, bool * compression)
  throw (Error)
{
  DJV_DEBUG("Header::load");
    
  // Read.
  
  io.get_u8(&_data.id_size);
  io.get_u8(&_data.cmap_type);
  io.get_u8(&_data.image_type);
  io.get_u16(&_data.cmap_start);
  io.get_u16(&_data.cmap_size);
  io.get_u8(&_data.cmap_bits);
  io.get_u16(&_data.x);
  io.get_u16(&_data.y);
  io.get_u16(&_data.width);
  io.get_u16(&_data.height);
  io.get_u8(&_data.pixel_bits);
  io.get_u8(&_data.descriptor);

  io.seek(_data.id_size);
  
  // Information.

  info.size = V2i(_data.width, _data.height);
  
  info.mirror = V2b(
    (_data.descriptor >> 4) & 1,
    (_data.descriptor >> 5) & 1
  );

  const int alpha_bits = _data.descriptor & 15;
  
  DJV_DEBUG_PRINT("alpha bits = " << alpha_bits);
  
  switch (alpha_bits)
  {
    case 0:
    case 8: break;
    default: io::throw_error_unsupported(name, io.file_name());
  }

  int pixel = -1;
  switch (_data.image_type)
  {
    case 3:
    case 11:
      switch (_data.pixel_bits)
      {
        case 8: pixel = pixel::L_U8; break;
        case 16: pixel = pixel::LA_U8; break;
      }
      break;

    case 2:
    case 10:
      switch (_data.pixel_bits)
      {
        case 24: pixel = pixel::RGB_U8; info.bgr = true; break;
        case 32: pixel = pixel::RGBA_U8; info.bgr = true; break;
      }
      break;
  }
  
  if (-1 == pixel)
    io::throw_error_unsupported(name, io.file_name());
  
  info.pixel = PIXEL(pixel);
  
  DJV_DEBUG_PRINT("pixel = " << info.pixel);
  
  const int bits = _data.pixel_bits + alpha_bits;
  
  if (bits < (pixel::channels(info.pixel) * 8) || (bits % 8) != 0)
    io::throw_error_unsupported(name, io.file_name());
  
  *compression =
    10 == _data.image_type ||
    11 == _data.image_type;

  DJV_DEBUG_PRINT("compression = " << compression);
  
  info.endian = memory::LSB;
}

void Header::save(File_Io & io, const io::Info & info, bool compression)
  throw (Error)
{
  DJV_DEBUG("Header::save");
  
  // Information.
  
  _data.width = info.size.x;
  _data.height = info.size.y;

  const int channels = pixel::channels(info.pixel);
  _data.pixel_bits = channels * 8;
  switch (channels)
  {
    case 1:
    case 2: _data.image_type = 3; break;
    case 3:
    case 4: _data.image_type = 2; break;
  }
  
  if (compression)
    _data.image_type += 8;
  
  int alpha_bits = 0;
  switch (pixel::format(info.pixel))
  {
    case pixel::LA:
    case pixel::RGBA: alpha_bits = 8; break;
    default: break;
  }
  
  const int orient = 0;
  
  _data.descriptor = (alpha_bits & 15) | ((orient & 3) << 4);

  // Write.
  
  io.set_u8(_data.id_size);
  io.set_u8(_data.cmap_type);
  io.set_u8(_data.image_type);
  io.set_u16(_data.cmap_start);
  io.set_u16(_data.cmap_size);
  io.set_u8(_data.cmap_bits);
  io.set_u16(_data.x);
  io.set_u16(_data.y);
  io.set_u16(_data.width);
  io.set_u16(_data.height);
  io.set_u8(_data.pixel_bits);
  io.set_u8(_data.descriptor);
}

void Header::debug() const
{
  DJV_DEBUG("Header::debug");
  DJV_DEBUG_PRINT("id size = " << _data.id_size);
  DJV_DEBUG_PRINT("cmap type = " << _data.cmap_type);
  DJV_DEBUG_PRINT("image type = " << _data.image_type);
  DJV_DEBUG_PRINT("cmap start = " << _data.cmap_start);
  DJV_DEBUG_PRINT("cmap size = " << _data.cmap_size);
  DJV_DEBUG_PRINT("cmap bits = " << _data.cmap_bits);
  DJV_DEBUG_PRINT("image position = " <<_data. x << " " << _data.y);
  DJV_DEBUG_PRINT("image size = " << _data.width << " " << _data.height);
  DJV_DEBUG_PRINT("pixel bits = " << _data.pixel_bits);
}

}

//------------------------------------------------------------------------------
// info_load(), info_save()
//------------------------------------------------------------------------------

void info_load(File_Io & io, io::Info & info, bool * compression)
  throw (Error)
{
  Header header;
  header.load(io, info, compression);
}

void info_save(File_Io & io, const io::Info & info, bool compression)
  throw (Error)
{
  Header header;
  header.save(io, info, compression);
}

//------------------------------------------------------------------------------

}
}
