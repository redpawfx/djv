/*------------------------------------------------------------------------------
 plugin/djv_png/save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "save.h"

#include <djv_image/gl_data.h>

namespace djv_plugin {
namespace png {

extern "C" {

//------------------------------------------------------------------------------
// djv_image_save()
//------------------------------------------------------------------------------

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_save()
{
  return new Save;
}

}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::Save() :
  _f(0),
  _png(0),
  _png_info(0)
{}

Save::~Save()
{
  _close();
}

plugin::Plugin * Save::copy() const
{
  return new Save;
}

String Save::name() const
{
  return png::name;
}

List<String> Save::extension_list() const
{
  return png::extension_list;
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
    case pixel::F16: 
    case pixel::F32: type = pixel::U16; break;
    default: break;
  }
  _info.pixel = pixel::pixel(pixel::format(info.pixel), type);
  
  DJV_DEBUG_PRINT("info = " << _info);
  
  _image.set(_info);
}

namespace {

bool open(
  FILE * f,
  png_structp png,
  png_infop * png_info,
  const io::Info & info
) {
  if (setjmp(png_jmpbuf(png)))
    return false;

  *png_info = png_create_info_struct(png);
  if (! *png_info)
    return false;
  
  png_init_io(png, f);
  
  int color_type = 0;
  switch (info.pixel)
  {
    case pixel::L_U8:
    case pixel::L_U16: color_type = PNG_COLOR_TYPE_GRAY; break;
    case pixel::LA_U8:
    case pixel::LA_U16: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
    case pixel::RGB_U8:
    case pixel::RGB_U16: color_type = PNG_COLOR_TYPE_RGB; break;
    case pixel::RGBA_U8:
    case pixel::RGBA_U16: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
    default: break;
  }
  
  png_set_IHDR(
    png,
    *png_info,
    info.size.x,
    info.size.y,
    pixel::bit_depth(info.pixel),
    color_type,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  
  png_write_info(png, *png_info);
  
  return true;
}

}

void Save::_open(const String & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);

  _close();
  
  // Initialize libpng.
  
  _png = png_create_write_struct(
    PNG_LIBPNG_VER_STRING,
    0,
    djv_png_error,
    djv_png_warning
  );
  if (! _png)
    throw Error(name(), _png_error.msg);

  _png->error_ptr = &_png_error;

  SNPRINTF(
    _png_error.msg,
    string::cstring_size,
    String(string::Format(djv_image::io::error_open).arg(in)).c_str()
  );
  
  // Open.

#if defined(DJV_WINDOWS)
  ::fopen_s(&_f, in.c_str(), "wb");
#else
  _f = ::fopen(in.c_str(), "wb");
#endif
  if (! _f)
    djv_image::io::throw_error_open(name(), in);

  if (! png::open(_f, _png, &_png_info, info))
    throw Error(name(), _png_error.msg);
}

void Save::_close()
{
  if (_png || _png_info)
  {
    png_destroy_write_struct(
      _png ? &_png : 0,
      _png_info ? &_png_info : 0);
    _png = 0;
    _png_info = 0;
  }
  
  if (_f)
  {
    ::fclose(_f);
    _f = 0;
  }
}

//------------------------------------------------------------------------------
// Save::set()
//------------------------------------------------------------------------------

namespace {

bool scanline(png_structp png, const uint8_t * in)
{
  if (setjmp(png_jmpbuf(png)))
    return false;
  
  png_write_row(png, (png_byte *)in);
  
  return true;
}

bool end(png_structp png, png_infop png_info)
{
  if (setjmp(png_jmpbuf(png)))
    return false;
  
  png_write_end(png, png_info);
  
  return true;
}

}

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);

  // Open file.
  
  const String file_name = _file.get(frame.frame);
  
  DJV_DEBUG_PRINT("file name = " << file_name);
  
  io::Info info(_info);
  info.tag = in.tag;
  
  _open(file_name, info);
  
  // Temporary buffers.
  
  const Data * p = &in;
  
  if (in.info() != _image.info())
  {
    DJV_DEBUG_PRINT("convert = " << _image);
    _image.zero();
    gl_data::copy(in, &_image);
    p = &_image;
  }
  
  // Write.
  
  const int h = p->h();
  for (int y = 0; y < h; ++y)
    if (! scanline(_png, p->data(0, h - 1 - y)))
      throw Error(name(), _png_error.msg);

  if (! end(_png, _png_info))
    throw Error(name(), _png_error.msg);

  _close();
}

//------------------------------------------------------------------------------

}
}

