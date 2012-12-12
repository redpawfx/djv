/*------------------------------------------------------------------------------
 lib/djv_image/io_lut_save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_lut.h"

#include <djv_image/gl_data.h>

namespace djv_image {
namespace io_lut {

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return io_lut::name;
}

List<String> Save::extension_list() const
{
  return io_lut::extension_list;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_value(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  try
  {
    if (string::compare_no_case(in, option_list()[SAVE_FORMAT]))
      *data >> _option.format;
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
  
  if (string::compare_no_case(in, option_list()[SAVE_FORMAT]))
    out << _option.format;
  
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
      case SAVE_FORMAT: out = label_format(); break;
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
      case SAVE_FORMAT: out << _option.format; break;
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
  _info.size = V2i(info.size.x, 1);
  pixel::TYPE type = pixel::type(info.pixel);
  switch (type)
  {
#if defined (DJV_OPENEXR)
    case pixel::F16: 
#endif
    case pixel::F32: type = pixel::U16; break;
    default: break;
  }
  _info.pixel = pixel::pixel(pixel::format(info.pixel), type);
  
  DJV_DEBUG_PRINT("info = " << _info);
  
  _image.set(_info);
}

void Save::_open(const File & in) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  _io.open(in, file_io::WRITE);
  
  _format = _option.format;
  if (FORMAT_AUTO == _format)
  {
    if (in.extension() == io_lut::extension_list[0])
      _format = FORMAT_INFERNO;
    else
      _format = FORMAT_KODAK;
  }
  
  switch (_format)
  {
    case FORMAT_INFERNO:
      inferno_open(_io, _info);
      break;
    
    case FORMAT_KODAK:
      kodak_open(_io, _info);
      break;
    
    default: break;
  }
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);
  DJV_DEBUG_PRINT("frame = " << frame);
  
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
  
  switch (_format)
  {
    case FORMAT_INFERNO:
      inferno_save(_io, p);
      break;
    
    case FORMAT_KODAK:
      kodak_save(_io, p);
      break;
    
    default: break;
  }
  
  _io.close();
}

//------------------------------------------------------------------------------

}
}

