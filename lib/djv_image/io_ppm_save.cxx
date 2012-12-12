/*------------------------------------------------------------------------------
 lib/djv_image/io_ppm_save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_ppm.h"

#include <djv_image/gl_data.h>

namespace djv_image {
namespace io_ppm {

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save()
{
  _io.endian(memory::endian() != memory::MSB);
}

plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return io_ppm::name;
}

List<String> Save::extension_list() const
{
  return io_ppm::extension_list;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_value(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  try
  {
    if (string::compare_no_case(in, option_list()[SAVE_TYPE]))
      *data >> _option.type;
    else if (string::compare_no_case(in, option_list()[SAVE_DATA]))
      *data >> _option.data;
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
  
  if (string::compare_no_case(in, option_list()[SAVE_TYPE]))
    out << _option.type;
  else if (string::compare_no_case(in, option_list()[SAVE_DATA]))
    out << _option.data;
  
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
      case SAVE_TYPE: out = label_type(); break;
      case SAVE_DATA: out = label_data(); break;
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
      case SAVE_TYPE: out << _option.type; break;
      case SAVE_DATA: out << _option.data; break;
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
  _info.mirror.y = true;
  
  switch (_option.type)
  {
    case TYPE_AUTO:
    {
      pixel::FORMAT format = pixel::format(info.pixel);
      switch (format)
      {
        case pixel::LA: format = pixel::L; break;
        case pixel::RGBA: format = pixel::RGB; break;
        default: break;
      }
      pixel::TYPE type = pixel::type(info.pixel);
      switch (type)
      {
        case pixel::U10:
#if defined (DJV_OPENEXR)
        case pixel::F16: 
#endif
        case pixel::F32: type = pixel::U16; break;
        default: break;
      }
      _info.pixel = pixel::pixel(format, type);
      _bit_depth = pixel::bit_depth(_info.pixel);
    }
    break;
    
    case TYPE_U1:
      _info.pixel = pixel::L_U8;
      _bit_depth = 1;
      break;
  }
  
  _info.endian = memory::MSB;
  
  DJV_DEBUG_PRINT("info = " << _info);

  _image.set(_info);
}

void Save::_open(const String & in) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  // Open.
  
  _io.open(in, file_io::WRITE);
  
  // Header.
  
  int ppm_type = 0;
  if (1 == _bit_depth)
  {
    ppm_type = DATA_ASCII == _option.data ? 1 : 4;
  }
  else
  {
    ppm_type = DATA_ASCII == _option.data ? 2 : 5;
    if (3 == _image.channels())
      ++ppm_type;
  }
  
  char magic [] = "P \n";
  magic[1] = '0' + ppm_type;
  _io.set(magic, 3);
  
  char tmp [string::cstring_size] = "";
  int size = SNPRINTF(
    tmp,
    string::cstring_size, "%d %d\n",
    _image.w(),
    _image.h()
  );
  _io.set(tmp, size);
  
  if (_bit_depth != 1)
  {
    const int max_value = (8 == _bit_depth) ? pixel::u8_max : pixel::u16_max;
    size = SNPRINTF(tmp, string::cstring_size, "%d\n", max_value);
    _io.set(tmp, size);
  }
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("type = " << _option.type);
  DJV_DEBUG_PRINT("data = " << _option.data);
  
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
  
  if (DATA_BINARY == _option.data && _bit_depth != 1)
  {
    _io.set(p->data(), p->bytes_data());
  }
  else
  {
    const int w = p->w(), h = p->h();
    const int channels = pixel::channels(p->info().pixel);
    const size_t bytes_scanline = io_ppm::bytes_scanline(w, channels,
      _bit_depth, _option.data);
    
    memory::Buffer<uint8_t> scanline(bytes_scanline);
    
    DJV_DEBUG_PRINT("scanline = " << static_cast<int>(bytes_scanline));
  
    for (int y = 0; y < h; ++y)
      if (DATA_BINARY == _option.data && 1 == _bit_depth)
      {
        const uint8_t * in_p = p->data(0, y);
        uint8_t * out_p = scanline();
        for (int i = 0; i < w; ++i)
        {
          const int tmp = in_p[i];
          const int off = i % 8;
          const int j = i / 8;
          if (0 == off) out_p[j] = 0;
          out_p[j] |= ((! tmp) & 1) << (7 - off);
        }
        _io.set(scanline(), bytes_scanline);
      }
      else
      {
        const size_t size = ascii_save(p->data(0, y), scanline(), w * channels,
          _bit_depth);
        _io.set(scanline(), size);
      }
  }
  
  _io.close();
}

//------------------------------------------------------------------------------

}
}
