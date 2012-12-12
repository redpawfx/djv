/*------------------------------------------------------------------------------
 lib/djv_image/io_sgi_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_sgi.h"

namespace djv_image {
namespace io_sgi {

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load()
{
  _io.endian(memory::endian() != memory::MSB);
}

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return io_sgi::name;
}

List<String> Load::extension_list() const
{
  return io_sgi::extension_list;
}

//------------------------------------------------------------------------------
// Load::open(), Load::_open()
//------------------------------------------------------------------------------

void Load::open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::open");
  DJV_DEBUG_PRINT("in = " << in);

  _file = in;
  _open(_file.get(_file.seq().start()), info);
  
  if (file::SEQ == _file.type())
    info.time.list = _file.seq().list;
}

void Load::_open(const String & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::_open");
  DJV_DEBUG_PRINT("in = " << in);
  
  _io.open(in, file_io::READ);
  
  info.file_name = in;
  info_load(_io, info, &_compression);

  // Scanline tables.
  
  if (_compression)
  {
    const int table_size = info.size.y * pixel::channels(info.pixel);
    DJV_DEBUG_PRINT("rle table size = " << table_size);
  
    _rle_offset.size(table_size);
    _rle_size.size(table_size);

    _io.get_u32(_rle_offset(), table_size);
    _io.get_u32(_rle_size(), table_size);
  }
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

const Image * Load::get(const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Load::get");
  DJV_DEBUG_PRINT("frame = " << frame);
  
  // Open.
  
  const String file_name =
    _file.get(frame.frame != -1 ? frame.frame : _file.seq().start());
  
  DJV_DEBUG_PRINT("file name = " << file_name);
  
  io::Info info;
  _open(file_name, info);
  
  // Read.
  
  _io.read_ahead();

  const size_t position = _io.position();
  const size_t size = _io.size() - position;
  
  const int channels = pixel::channels(info.pixel);
  const int bytes = pixel::channel_bytes(info.pixel);

  if (! _compression)
  {
    if (1 == bytes)
    {
      const uint8_t * p = _io.mmap_p();
      _io.seek(data::bytes_data(info));
      _tmp.set(info, p);
    }
    else
    {
      _tmp.set(info);
      _io.get(_tmp.data(), size / bytes, bytes);
    }
  }
  else
  {
    _tmp.set(info);
    
    memory::Buffer<uint8_t> tmp(size);
    _io.get(tmp(), size / bytes, bytes);

    const uint8_t * in_p = tmp();
    const uint8_t * end = in_p + size;
    uint8_t * out_p = _tmp.data();

    for (int c = 0; c < channels; ++c)
    {
      DJV_DEBUG_PRINT("channel = " << c);

      for (int y = 0; y < info.size.y; ++y, out_p += info.size.x * bytes)
      {
        DJV_DEBUG_PRINT("y = " << y);
        
        if (
          ! rle_load(
            in_p + _rle_offset()[y + info.size.y * c] - position, end, out_p,
            info.size.x, bytes, _io.endian()
          )
        )
          io::throw_error_read(name(), file_name);
      }
    }
  }
  
  // Interleave channels.
  
  info.size = data::proxy_scale(info.size, frame.proxy);
  info.proxy = frame.proxy;
  _image.set(info);
  
  data::planar_interleave(_tmp, &_image, frame.proxy);
  
  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}
