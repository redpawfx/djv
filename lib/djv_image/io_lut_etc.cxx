/*------------------------------------------------------------------------------
 lib/djv_image/io_lut_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "io_lut.h"

#include <djv_image/color.h>

namespace djv_image {
namespace io_lut {

//------------------------------------------------------------------------------
// inferno_open()
//------------------------------------------------------------------------------

namespace {

int _bit_depth(File_Io & io) throw (Error)
{
  int out = 0;

  const size_t position = io.position();

  char tmp [string::cstring_size] = "";
  while (1)
  {
    try
    {
      file_io::word(io, tmp, string::cstring_size);
    }
    catch (Error) { break; }
    
    out = math::max(string::string_to_int<int>(tmp, string::cstring_size), out);
  }

  io.position(position);
  
  if (out <= pixel::u8_max)
    return 8;
  else if (out <= pixel::u10_max)
    return 10;
  return 16;
}

}

void inferno_open(File_Io & io, Info & info, TYPE type) throw (Error)
{
  DJV_DEBUG("inferno_open");
    
  // Header.
  
  char tmp [string::cstring_size] = "";
  file_io::word(io, tmp, string::cstring_size);
  
  DJV_DEBUG_PRINT("magic = " << tmp);
  
  if (String(tmp) != "LUT:")
    io::throw_error_unrecognized(name, io.file_name());
  
  file_io::word(io, tmp, string::cstring_size);
  const int channels = string::string_to_int<int>(tmp, string::cstring_size);
  
  file_io::word(io, tmp, string::cstring_size);
  const int size = string::string_to_int<int>(tmp, string::cstring_size);
  
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("channels = " << channels);
  
  // Information.
  
  info.size = V2i(size, 1);
  
  int bit_depth = 0;
  
  switch (type)
  {
    case TYPE_AUTO: bit_depth = _bit_depth(io); break;
    case TYPE_U8: bit_depth = 8; break;
    case TYPE_U10: bit_depth = 10; break;
    case TYPE_U16: bit_depth = 16; break;
  }
  
  DJV_DEBUG_PRINT("bit depth = " << bit_depth);
  
  if (! pixel::pixel_(channels, bit_depth, false, &info.pixel))
    io::throw_error_unsupported(name, io.file_name());
}

void inferno_open(File_Io & io, const Info & info) throw (Error)
{
  char tmp [string::cstring_size] = "";
  int size = SNPRINTF(tmp, string::cstring_size, "LUT: %d %d\n\n",
    pixel::channels(info.pixel), info.size.x);
  
  io.set(tmp, size);
}

//------------------------------------------------------------------------------
// kodak_open()
//------------------------------------------------------------------------------

void kodak_open(File_Io & io, Info & info, TYPE type) throw (Error)
{
  DJV_DEBUG("kodak_open");
  
  // Header.
  
  const size_t position = io.position();
  String header;
  int8_t c = 0;
  bool comment = false;
  
  while (1)
  {
    try
    {
      io.get_8(&c);
    }
    catch (Error) { break; }
    
    if ('#' == c)
      comment = true;
    
    else if ('\n' == c)
    {
      if (comment)
        comment = false;
      else
        break;
    }
    else
    {
      if (! comment)
        header += c;
    }
  }
  DJV_DEBUG_PRINT("header = " << header);
  
  int channels = static_cast<int>(
    string::split(header, List<char>() << ' ' << '\t').size());
  int size = 1;
  comment = false;
  
  while (1)
  {
    try
    {
      io.get_8(&c);
    }
    catch (Error) { break; }
    
    if ('\n' == c)
      ++size;
  }
  
  io.position(position);
  
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("channels = " << channels);
  
  // Information.
  
  info.size = V2i(size, 1);
  
  int bit_depth = 0;
  switch (type)
  {
    case TYPE_AUTO: bit_depth = _bit_depth(io); break;
    case TYPE_U8: bit_depth = 8; break;
    case TYPE_U10: bit_depth = 10; break;
    case TYPE_U16: bit_depth = 16; break;
  }
  
  DJV_DEBUG_PRINT("bit depth = " << bit_depth);
  
  if (! pixel::pixel_(channels, bit_depth, true, &info.pixel))
    io::throw_error_unsupported(name, io.file_name());
}

void kodak_open(File_Io &, const Info &) throw (Error)
{}

//------------------------------------------------------------------------------
// inferno_load()
//------------------------------------------------------------------------------

void inferno_load(File_Io & io, Image * out) throw (Error)
{
  DJV_DEBUG("inferno_load");
  
  List<Color> color(out->w());
  for (int x = 0; x < out->w(); ++x)
    color[x].pixel(out->pixel());
    
  for (int c = 0; c < pixel::channels(out->pixel()); ++c)
    for (int x = 0; x < out->w(); ++x)
    {
      char tmp [string::cstring_size] = "";
      file_io::word(io, tmp, string::cstring_size);
      const int v = string::string_to_int<int>(tmp, string::cstring_size);
      
      switch (pixel::type(out->pixel()))
      {
        case pixel::U8: color[x].set_u8(v, c); break;
        case pixel::U10: color[x].set_u10(v, c); break;
        case pixel::U16: color[x].set_u16(v, c); break;
        default: break;
      }
    }
  
  for (int x = 0; x < out->w(); ++x)
  {
    DJV_DEBUG_PRINT(x << " = " << color[x]);
    memory::copy(color[x].data(), out->data(x, 0), pixel::bytes(out->pixel()));
  }
}

//------------------------------------------------------------------------------
// kodak_load()
//------------------------------------------------------------------------------

void kodak_load(File_Io & io, Image * out) throw (Error)
{
  DJV_DEBUG("kodak_load");
  
  for (int x = 0; x < out->w(); ++x)
  {
    Color color(out->pixel());
    for (int c = 0; c < pixel::channels(out->pixel()); ++c)
    {
      char tmp [string::cstring_size] = "";
      file_io::word(io, tmp, string::cstring_size);
      const int v = string::string_to_int<int>(tmp, string::cstring_size);
      
      switch (pixel::type(out->pixel()))
      {
        case pixel::U8: color.set_u8(v, c); break;
        case pixel::U10: color.set_u10(v, c); break;
        case pixel::U16: color.set_u16(v, c); break;
        default: break;
      }
    }
    
    DJV_DEBUG_PRINT(x << " = " << color);
    
    memory::copy(color.data(), out->data(x, 0), pixel::bytes(out->pixel()));
  }
}

//------------------------------------------------------------------------------
// inferno_save()
//------------------------------------------------------------------------------

void inferno_save(File_Io & io, const Data * out) throw (Error)
{
  List<Color> color(out->w());
  for (int x = 0; x < out->w(); ++x)
  {
    color[x].pixel(out->pixel());
    memory::copy(out->data(x, 0), color[x].data(), pixel::bytes(out->pixel()));
  }
  
  for (int c = 0; c < pixel::channels(out->pixel()); ++c)
    for (int x = 0; x < out->w(); ++x)
    {
      int v = 0;
      
      switch (pixel::type(out->pixel()))
      {
        case pixel::U8: v = color[x].get_u8(c); break;
        case pixel::U10: v = color[x].get_u10(c); break;
        case pixel::U16: v = color[x].get_u16(c); break;
        default: break;
      }
      
      char tmp [string::cstring_size] = "";
      int size = SNPRINTF(tmp, string::cstring_size, "%9d\n", v);
      
      io.set(tmp, size);
    }
}

//------------------------------------------------------------------------------
// kodak_save()
//------------------------------------------------------------------------------

void kodak_save(File_Io & io, const Data * out) throw (Error)
{
  for (int x = 0; x < out->w(); ++x)
  {
    Color color(out->pixel());
    memory::copy(out->data(x, 0), color.data(), pixel::bytes(out->pixel()));
    
    for (int c = 0; c < pixel::channels(out->pixel()); ++c)
    {
      int v = 0;
      
      switch (pixel::type(out->pixel())) {
        case pixel::U8: v = color.get_u8(c); break;
        case pixel::U10: v = color.get_u10(c); break;
        case pixel::U16: v = color.get_u16(c); break;
        default: break;
      }
      
      char tmp [string::cstring_size] = "";
      int size = SNPRINTF(tmp, string::cstring_size, "%6d", v);
      io.set(tmp, size);
    }
    
    io.set_8('\n');
  }
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(FORMAT, label_format())
_DJV_STRING_OPERATOR_LABEL(TYPE, label_type())

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_format()
{
  static const List<String> data = List<String>() <<
    "Auto" <<
    "Inferno" <<
    "Kodak";
  
  DJV_ASSERT(data.size() == _FORMAT_SIZE);

  return data;
}

const List<String> & label_type()
{
  static const List<String> data = List<String>() <<
    "Auto" <<
    "U8" <<
    "U10" <<
    "U16";

  DJV_ASSERT(data.size() == _TYPE_SIZE);

  return data;
}

const List<String> & label_load_option()
{
  static const List<String> data = List<String>() <<
    "File format" <<
    "File type";

  return data;
}

const List<String> & label_save_option()
{
  static const List<String> data = List<String>() <<
    "File format";

  return data;
}

//------------------------------------------------------------------------------

}
}
