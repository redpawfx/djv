/*------------------------------------------------------------------------------
 lib/djv_image/io_sgi_save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_sgi.h"

#include <djv_image/gl_data.h>

namespace djv_image {
namespace io_sgi {

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
  return io_sgi::name;
}

List<String> Save::extension_list() const
{
  return io_sgi::extension_list;
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
  
  _info.pixel = pixel::pixel(pixel::format(info.pixel), type);
  _info.endian = memory::MSB;
  
  DJV_DEBUG_PRINT("info = " << _info);

  _image.set(_info);
}

void Save::_open(const String & in) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  _io.open(in, file_io::WRITE);

  info_save(_io, _info, _option.compression != COMPRESSION_NONE);
  
  // Scanline tables.

  if (_option.compression)
  {
    const int table_size = _info.size.y * pixel::channels(_info.pixel);
    DJV_DEBUG_PRINT("rle table size = " << table_size);
    
    _rle_offset.size(table_size);
    _rle_size.size(table_size);
    
    _io.seek(table_size * 2 * 4);
  }
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("compression = " << _option.compression);

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
  
  _tmp.set(p->info());

  const int w = _tmp.w(), h = _tmp.h();
  const int channels = pixel::channels(_tmp.pixel());
  const int bytes = pixel::channel_bytes(_tmp.pixel());

  // Deinterleave channels.
  
  data::planar_deinterleave(*p, &_tmp);
  
  // Write.

  if (! _option.compression)
  {
    _io.set(_tmp.data(), _tmp.bytes_data() / bytes, bytes);
  }
  else
  {
    memory::Buffer<uint8_t> scanline(w * bytes * 2);

    for (int c = 0; c < channels; ++c)
      for (int y = 0; y < h; ++y)
      {
        const size_t size = rle_save(_tmp.data() + (c * h + y) * w * bytes,
          scanline(), w, bytes, _io.endian());

        _rle_offset()[y + c * h] = uint32_t(_io.position());
        _rle_size()[y + c * h] = uint32_t(size);

        _io.set(scanline.data(), size / bytes, bytes);
      }

    _io.position(512);
    _io.set_u32(_rle_offset(), h * channels);
    _io.set_u32(_rle_size(), h * channels);
  }

  _io.close();
}

//------------------------------------------------------------------------------

}
}
