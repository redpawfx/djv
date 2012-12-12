/*------------------------------------------------------------------------------
 lib/djv_image/io_cineon_load.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_cineon.h"

namespace djv_image {
namespace io_cineon {

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _film_print(false)
{}

plugin::Plugin * Load::copy() const
{
  Load * out = new Load;
  out->_option = _option;
  return out;
}

String Load::name() const
{
  return io_cineon::name;
}

List<String> Load::extension_list() const
{
  return io_cineon::extension_list;
}

//------------------------------------------------------------------------------
// Load::option(), Load::option_list(), Load::option_value(),
// Load::option_default()
//------------------------------------------------------------------------------

bool Load::option(const String & in, String * data)
{
  _film_print_lut.reset();
  
  try
  {
    if (string::compare_no_case(in, option_list()[LOAD_COLOR_PROFILE]))
      *data >>_option.color_profile;
    else if (string::compare_no_case(in, option_list()[LOAD_FILM_PRINT]))
      *data >> _option.film_print;
    else if (string::compare_no_case(in, option_list()[LOAD_CONVERT]))
      *data >> _option.convert;
  }
  catch (String)
  {
    return false;
  }
  
  return true;
}

String Load::option(const String & in) const
{
  String out;
  
  if (string::compare_no_case(in, option_list()[LOAD_COLOR_PROFILE]))
    out << _option.color_profile;
  else if (string::compare_no_case(in, option_list()[LOAD_FILM_PRINT]))
    out << _option.film_print;
  else if (string::compare_no_case(in, option_list()[LOAD_CONVERT]))
    out << _option.convert;
  
  return out;
}

List<String> Load::option_list() const
{
  return label_load_option();
}

List<String> Load::option_value(const String & in) const
{
  List<String> out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_load_option(), &index))
    switch (index)
    {
      case LOAD_COLOR_PROFILE: out = label_color_profile(); break;
      case LOAD_FILM_PRINT: out += label_film_print_load; break;
      case LOAD_CONVERT: out = label_convert(); break;
    }
  
  return out;
}

String Load::option_default(const String & in) const
{
  String out;
  
  list::size_type index = 0;
  if (string::find_no_case(in, label_load_option(), &index))
    switch (index)
    {
      case LOAD_COLOR_PROFILE: out << _option.color_profile; break;
      case LOAD_FILM_PRINT: out << _option.film_print; break;
      case LOAD_CONVERT: out << _option.convert; break;
    }
  
  return out;
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
  _film_print = false;
  info_load(_io, info, _film_print);
  
  DJV_DEBUG_PRINT("info = " << info);
  DJV_DEBUG_PRINT("film print = " << _film_print);
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
  _image.tag = info.tag;

  if (
    (COLOR_PROFILE_FILM_PRINT == _option.color_profile) ||
    (COLOR_PROFILE_AUTO == _option.color_profile && _film_print)
  ) {
    DJV_DEBUG_PRINT("color profile");
    
    _image.color_profile.type = color_profile::LUT;
    
    if (! _film_print_lut.get())
      _film_print_lut =
        std::auto_ptr<Data>(new Data(film_unprint_lut(_option.film_print)));
    
    _image.color_profile.lut = *_film_print_lut;
  }
  else
  {
    _image.color_profile = Color_Profile();
  }
  
  const Frame_Cache frame_cache(file_name, frame);
  if (frame_cache == _frame_cache)
  {
    DJV_DEBUG_PRINT("cached");
    
    return &_image;
  }
  else
  {
    _frame_cache = frame_cache;
  }

  // Read.
  
  _io.read_ahead();
  
  const uint8_t * p = _io.mmap_p();
  
  _io.seek(data::bytes_data(info));
  
  if (! frame.proxy && CONVERT_NONE == _option.convert)
  {
    _image.set(info, p);
  }
  else
  {
    _tmp.set(info, p);
    
    info.size = data::proxy_scale(info.size, frame.proxy);
    info.proxy = frame.proxy;
    
    switch (_option.convert)
    {
      case CONVERT_NONE: break;
      
      case CONVERT_U8:
        info.pixel = pixel::pixel(info.pixel, pixel::U8);
        info.endian = memory::endian();
        break;
      
      case CONVERT_U16:
        info.pixel = pixel::pixel(info.pixel, pixel::U16);
        info.endian = memory::endian();
        break;
    }
    
    _image.set(info);
    
    data::proxy_scale(_tmp, &_image, frame.proxy);
  }

  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}
