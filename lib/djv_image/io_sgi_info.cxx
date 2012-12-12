/*------------------------------------------------------------------------------
 lib/djv_image/io_sgi_info.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_sgi.h"

namespace djv_image {
namespace io_sgi {

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
    uint16_t magic;
    uint8_t  storage;
    uint8_t  bytes;
    uint16_t dimension;
    uint16_t width;
    uint16_t height;
    uint16_t channels;
    uint32_t pixel_min;
    uint32_t pixel_max;
  } _data;
};

Header::Header()
{
  _data.magic = 474;
  _data.storage = 0;
  _data.bytes = 0;
  _data.dimension = 0;
  _data.width = 0;
  _data.height = 0;
  _data.channels = 0;
  _data.pixel_min = 0;
  _data.pixel_max = 0;
}

void Header::load(File_Io & io, io::Info & info, bool * compression)
  throw (Error)
{
  DJV_DEBUG("Header::load");
  
  // Read.
  
  io.get_u16(&_data.magic);
  if (_data.magic != 474)
    io::throw_error_unrecognized(name, io.file_name());
  
  io.get_u8(&_data.storage);
  io.get_u8(&_data.bytes);
  io.get_u16(&_data.dimension);
  io.get_u16(&_data.width);
  io.get_u16(&_data.height);
  io.get_u16(&_data.channels);
  io.get_u32(&_data.pixel_min);
  io.get_u32(&_data.pixel_max);
  
  io.position(512);
  
  // Information.

  info.size = V2i(_data.width, _data.height);

  if (
    ! pixel::pixel_(
      _data.channels,
      1 == _data.bytes ? 8 : 16,
      false,
      &info.pixel
    )
  )
    io::throw_error_unsupported(name, io.file_name());
  
  info.endian = memory::MSB;

  *compression = _data.storage ? true : false;
}

void Header::save(File_Io & io, const io::Info & info, bool compression)
  throw (Error)
{
  DJV_DEBUG("Header::save");
  
  // Information.
  
  const int channels = pixel::channels(info.pixel);
  const int bytes = pixel::channel_bytes(info.pixel);
  
  _data.width = info.size.x;
  _data.height = info.size.y;

  _data.channels = channels;
  _data.bytes = bytes;

  _data.dimension = 1 == channels ? (1 == info.size.y ? 1 : 2) : 3;

  _data.pixel_min = 0;
  _data.pixel_max = 1 == bytes ? 255 : 65535;

  _data.storage = compression;
  
  // Write.
  
  io.set_u16(_data.magic);
  io.set_u8(_data.storage);
  io.set_u8(_data.bytes);
  io.set_u16(_data.dimension);
  io.set_u16(_data.width);
  io.set_u16(_data.height);
  io.set_u16(_data.channels);
  io.set_u32(_data.pixel_min);
  io.set_u32(_data.pixel_max);
  
  io.position(512);
}

void Header::debug() const
{
  DJV_DEBUG("Header::debug");
  DJV_DEBUG_PRINT("magic = " << _data.magic);
  DJV_DEBUG_PRINT("storage = " << _data.storage);
  DJV_DEBUG_PRINT("bytes = " << _data.bytes);
  DJV_DEBUG_PRINT("dimension = " << _data.dimension);
  DJV_DEBUG_PRINT("width = " << _data.width);
  DJV_DEBUG_PRINT("height = " << _data.height);
  DJV_DEBUG_PRINT("pixel range = " << _data.pixel_min << " " << _data.pixel_max);
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

