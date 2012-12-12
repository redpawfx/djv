/*------------------------------------------------------------------------------
 lib/djv_image/io_dpx_save.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_dpx.h"

#include <djv_image/gl_data.h>

namespace djv_image {
namespace io_dpx {

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
  return io_dpx::name;
}

List<String> Save::extension_list() const
{
  return io_dpx::extension_list;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_value(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  try
  {
    if (string::compare_no_case(in, option_list()[SAVE_COLOR_PROFILE]))
      *data >> _option.color_profile;
    else if (string::compare_no_case(in, option_list()[SAVE_FILM_PRINT]))
      *data >> _option.film_print;
    else if (string::compare_no_case(in, option_list()[SAVE_VERSION]))
      *data >> _option.version;
    else if (string::compare_no_case(in, option_list()[SAVE_TYPE]))
      *data >> _option.type;
    else if (string::compare_no_case(in, option_list()[SAVE_ENDIAN]))
      *data >> _option.endian;
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
  
  if (string::compare_no_case(in, option_list()[SAVE_COLOR_PROFILE]))
    out << _option.color_profile;
  else if (string::compare_no_case(in, option_list()[SAVE_FILM_PRINT]))
    out << _option.film_print;
  else if (string::compare_no_case(in, option_list()[SAVE_VERSION]))
    out << _option.version;
  else if (string::compare_no_case(in, option_list()[SAVE_TYPE]))
    out << _option.type;
  else if (string::compare_no_case(in, option_list()[SAVE_ENDIAN]))
    out << _option.endian;
  
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
      case SAVE_COLOR_PROFILE: out = io_cineon::label_color_profile(); break;
      case SAVE_FILM_PRINT: out += io_cineon::label_film_print_save; break;
      case SAVE_VERSION: out = label_version(); break;
      case SAVE_TYPE: out = label_type(); break;
      case SAVE_ENDIAN: out = label_endian(); break;
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
      case SAVE_COLOR_PROFILE: out << _option.color_profile; break;
      case SAVE_FILM_PRINT: out << _option.film_print; break;
      case SAVE_VERSION: out << _option.version; break;
      case SAVE_TYPE: out << _option.type; break;
      case SAVE_ENDIAN: out << _option.endian; break;
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
  DJV_DEBUG_PRINT("info = " << info);
  
  _file = in;
  if (info.time.list.size() > 1)
    _file.type(file::SEQ);
  
  _info = io::Info();
  _info.size = info.size;
  _info.mirror.y = true;
  
  switch (_option.endian)
  {
    case ENDIAN_AUTO: break;
    case ENDIAN_MSB: _info.endian = memory::MSB; break;
    case ENDIAN_LSB: _info.endian = memory::LSB; break;
  }
  
  switch (_option.type)
  {
    case TYPE_U10:
      _info.pixel = pixel::RGB_U10;
      break;

    default:
    {
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
    }
    break;
  }
  
  switch (pixel::bit_depth(_info.pixel))
  {
    case 8:
    case 10: _info.align = 4; break;
    default: break;
  }
  
  DJV_DEBUG_PRINT("info = " << _info);

  _image.set(_info);
}

void Save::_open(const String & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  _io.open(in, file_io::WRITE);
  
  info_save(_io, _info, _option.endian, _option.color_profile, _option.version);
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Setup.
  
  Color_Profile color_profile;
  
  if (
    io_cineon::COLOR_PROFILE_FILM_PRINT == _option.color_profile ||
    io_cineon::COLOR_PROFILE_AUTO == _option.color_profile
  ) {
    DJV_DEBUG_PRINT("color profile");
    
    color_profile.type = color_profile::LUT;
    color_profile.lut = film_print_lut(_option.film_print);
  }

  // Open.
  
  const String file_name = _file.get(frame.frame);
  
  DJV_DEBUG_PRINT("file name = " << file_name);
  
  io::Info info(_info);
  info.file_name = file_name;
  info.tag = in.tag;
  
  _open(file_name, info);
  
  // Convert.

  const Data * p = &in;
  
  if (
    in.info() != _info ||
    in.color_profile.type != color_profile::RAW ||
    color_profile.type != color_profile::RAW
  ) {
    DJV_DEBUG_PRINT("convert = " << _image);
    
    _image.zero();
    
    gl_data::Option option;
    option.color_profile = color_profile;
    gl_data::copy(*p, &_image, option);
    
    p = &_image;
  }
  
  // Write.
  
  _io.set(p->data(), p->bytes_data());
  
  info_update(_io);
  
  _io.close();
}

//------------------------------------------------------------------------------

}
}

