/*------------------------------------------------------------------------------
 lib/djv_image/io_lut_load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_lut.h"

namespace djv_image {
namespace io_lut {

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

plugin::Plugin * Load::copy() const
{
  Load * out = new Load;
  out->_option = _option;
  return out;
}

String Load::name() const
{
  return io_lut::name;
}

List<String> Load::extension_list() const
{
  return io_lut::extension_list;
}

//------------------------------------------------------------------------------
// Load::option(), Load::option_list(), Load::option_value(),
// Load::option_default()
//------------------------------------------------------------------------------

bool Load::option(const String & in, String * data)
{
  try
  {
    if (string::compare_no_case(in, option_list()[LOAD_FORMAT]))
      *data >> _option.format;
    else if (string::compare_no_case(in, option_list()[LOAD_TYPE]))
      *data >> _option.type;
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
  
  if (string::compare_no_case(in, option_list()[LOAD_FORMAT]))
    out << _option.format;
  else if (string::compare_no_case(in, option_list()[LOAD_TYPE]))
    out << _option.type;
  
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
      case LOAD_FORMAT: out = label_format(); break;
      case LOAD_TYPE: out = label_type(); break;
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
      case LOAD_FORMAT: out << _option.format; break;
      case LOAD_TYPE: out << _option.type; break;
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

void Load::_open(const File & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::_open");
  DJV_DEBUG_PRINT("in = " << in);

  _io.open(in, file_io::READ);

  info.file_name = in;
  
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
      inferno_open(_io, info, _option.type);
      break;
    
    case FORMAT_KODAK:
      kodak_open(_io, info, _option.type);
      break;
    
    default: break;
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
  
  io::Info info;
  _open(file_name, info);
  
  // Read.
  
  _image.set(info);
  
  switch (_format)
  {
    case FORMAT_INFERNO:
      inferno_load(_io, &_image);
      break;
    
    case FORMAT_KODAK:
      kodak_load(_io, &_image);
      break;
    
    default: break;
  }
  
  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}
