/*------------------------------------------------------------------------------
 lib/djv_image/io_ppm_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_ppm.h"

namespace djv_image {
namespace io_ppm {

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
  return io_ppm::name;
}

List<String> Load::extension_list() const
{
  return io_ppm::extension_list;
}

//------------------------------------------------------------------------------
// Load::open(), Load::_open()
//------------------------------------------------------------------------------

void Load::open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::open");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("type = " << in.type());

  _file = in;
  _open(in.get(in.seq().start()), info);
  
  if (file::SEQ == _file.type())
    info.time.list = _file.seq().list;
}

void Load::_open(const String & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::_open");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Open.
  
  _io.open(in, file_io::READ);
  
  char magic [] = { 0, 0, 0 };
  _io.get(magic, 2);
  
  DJV_DEBUG_PRINT("magic = " << magic);
  
  if (magic[0] != 'P')
    io::throw_error_unrecognized(name(), in);

  switch (magic[1])
  {
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6': break;
    default: io::throw_error_unsupported(name(), in);
  }
  
  const int ppm_type = magic[1] - '0';
  
  DJV_DEBUG_PRINT("ppm type = " << ppm_type);
  
  // Header.

  char tmp [string::cstring_size] = "";

  int width = 0, height = 0;
  file_io::word(_io, tmp, string::cstring_size);
  width = string::string_to_int<int>(tmp, string::cstring_size);
  file_io::word(_io, tmp, string::cstring_size);
  height = string::string_to_int<int>(tmp, string::cstring_size);

  int max_value = 0;
  if (ppm_type != 1 && ppm_type != 4)
  {
    file_io::word(_io, tmp, string::cstring_size);
    max_value = string::string_to_int<int>(tmp, string::cstring_size);
  }
  
  DJV_DEBUG_PRINT("max value = " << max_value);
  
  // Information.
  
  info.file_name = in;
  
  info.size = V2i(width, height);
  
  info.mirror.y = true;

  if (1 == ppm_type || 4 == ppm_type)
    _bit_depth = 1;
  else
    _bit_depth = max_value < 256 ? 8 : 16;
  
  DJV_DEBUG_PRINT("bit depth = " << _bit_depth);
  
  int channels = 0;
  switch (ppm_type)
  {
    case 1:
    case 2:
    case 4:
    case 5: channels = 1; break;
    case 3:
    case 6: channels = 3; break;
  }

  if (
    ! pixel::pixel_(
      channels,
      _bit_depth != 1 ? _bit_depth : 8,
      false,
      &info.pixel
    )
  )
    io::throw_error_unsupported(name(), in);

  _data =
    (1 == ppm_type || 2 == ppm_type || 3 == ppm_type) ?
    DATA_ASCII :
    DATA_BINARY;
  
  if (DATA_BINARY == _data)
    info.endian = memory::MSB;
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
  
  Data * data = frame.proxy ? &_tmp : &_image;

  if (DATA_BINARY == _data && _bit_depth != 1)
  {
    const uint8_t * p = _io.mmap_p();
    _io.seek(data::bytes_data(info));
    data->set(info, p);
  }
  else
  {
    data->set(info);
    const int channels = pixel::channels(info.pixel);
    if (DATA_BINARY == _data && 1 == _bit_depth)
    {
      const size_t bytes_scanline = io_ppm::bytes_scanline(
        info.size.x, channels, _bit_depth, _data);
      memory::Buffer<uint8_t> scanline(bytes_scanline);
      
      DJV_DEBUG_PRINT("scanline = " << static_cast<int>(bytes_scanline));

      for (int y = 0; y < info.size.y; ++y)
      {
        _io.get(scanline(), bytes_scanline);
        const uint8_t * in_p = scanline();
        uint8_t * out_p = data->data(0, y);
        for (int i = info.size.x - 1; i >= 0; --i)
          out_p[i] = (in_p[i / 8] >> (7 - (i % 8))) & 1 ? 0 : 255;
      }
    }
    else
    {
      for (int y = 0; y < info.size.y; ++y)
        ascii_load(_io, data->data(0, y), info.size.x * channels, _bit_depth);
    }
  }
  
  if (frame.proxy)
  {
    info.size = data::proxy_scale(info.size, frame.proxy);
    info.proxy = frame.proxy;
    _image.set(info);
    
    data::proxy_scale(_tmp, &_image, frame.proxy);
  }

  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}

