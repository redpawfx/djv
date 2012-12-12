/*------------------------------------------------------------------------------
 lib/djv_image/io_targa_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_targa.h"

namespace djv_image {
namespace io_targa {

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load()
{
  _io.endian(memory::endian() != memory::LSB);
}

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return io_targa::name;
}

List<String> Load::extension_list() const
{
  return io_targa::extension_list;
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
  
  if (! _compression)
  {
    const uint8_t * p = _io.mmap_p();
    _io.seek(data::bytes_data(info));
    data->set(info, p);
    
    //data->set(info);
    //_io.get(data->data(), data->bytes_data());
  }
  else
  {
    data->set(info);

    const uint8_t * p = _io.mmap_p(), * const end = _io.mmap_end();
    const int channels = pixel::channels(info.pixel);
    
    for (int y = 0; y < info.size.y; ++y)
    {
      DJV_DEBUG_PRINT("y = " << y);
      
      p = rle_load(p, end, data->data(0, y), info.size.x, channels);
      
      if (! p)
        io::throw_error_read(name(), file_name);
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
