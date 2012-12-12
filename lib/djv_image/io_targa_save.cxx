/*------------------------------------------------------------------------------
 lib/djv_image/io_targa_save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_targa.h"

#include <djv_image/gl_data.h>

namespace djv_image {
namespace io_targa {

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save()
{
  _io.endian(memory::endian() != memory::LSB);
}

plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return io_targa::name;
}

List<String> Save::extension_list() const
{
  return io_targa::extension_list;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_value(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  try
  {
    if (string::compare_no_case(in, option_list()[SAVE_COMPRESSION]))
      *data >> _option.compression;
  }
  catch (String)
  {
    return false;
  }
  
  return true;
}

String Save::option(const String & in) const
{
  String out;
  
  if (string::compare_no_case(in, option_list()[SAVE_COMPRESSION]))
    out << _option.compression;
  
  return out;
}

List<String> Save::option_list() const
{
  return label_save_option();
}

List<String> Save::option_value(const String & in) const
{
  List<String> out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_save_option(), &index))
    switch (index)
    {
      case SAVE_COMPRESSION: out = label_compression(); break;
    }
  
  return out;
}

String Save::option_default(const String & in) const
{
  String out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_save_option(), &index))
    switch (index)
    {
      case SAVE_COMPRESSION: out << _option.compression; break;
    }
  
  return out;
}

//------------------------------------------------------------------------------
// Save::open(), Save::_open()
//------------------------------------------------------------------------------

void Save::open(const File & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::open");
  DJV_DEBUG_PRINT("in = " << in);
  
  _file = in;
  if (info.time.list.size() > 1)
    _file.type(file::SEQ);
  
  _info = Info();
  _info.size = info.size;
  _info.pixel = pixel::pixel(pixel::format(info.pixel), pixel::U8);
  
  switch (pixel::format(_info.pixel))
  {
    case pixel::RGB:
    case pixel::RGBA: _info.bgr = true; break;
    default: break;
  }
  _info.endian = memory::LSB;
  
  DJV_DEBUG_PRINT("info = " << _info);
  
  _image.set(_info);
}

void Save::_open(const String & in) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  _io.open(in, file_io::WRITE);
  
  info_save(_io, _info, _option.compression != COMPRESSION_NONE);
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);

  // Open.
  
  _open(_file.get(frame.frame));
  
  // Convert.
  
  const Data * p = &in;
  
  if (in.info() != _info)
  {
    DJV_DEBUG_PRINT("convert = " << _image);
    
    _image.zero();
    
    gl_data::copy(in, &_image);
    
    p = &_image;
  }
  
  // Write.
  
  if (! _option.compression)
  {
    _io.set(p->data(), p->bytes_data());
  }
  else
  {
    const int w = p->w(), h = p->h();
    const int channels = pixel::channels(p->info().pixel);
    
    memory::Buffer<uint8_t> scanline(w * channels * 2);

    for (int y = 0; y < h; ++y)
    {
      const size_t size = rle_save(p->data(0, y), scanline(), w, channels);
      _io.set(scanline.data(), size);
    }
  }

  _io.close();
}

//------------------------------------------------------------------------------

}
}
