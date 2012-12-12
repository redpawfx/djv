/*------------------------------------------------------------------------------
 plugin/djv_jpeg/save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "save.h"

#include <djv_image/gl_data.h>

namespace djv_plugin {
namespace jpeg {
using libjpeg::jpeg_compress_struct;

//------------------------------------------------------------------------------
// djv_image_save()
//------------------------------------------------------------------------------

extern "C" {

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
  _jpeg_init(false)
{}

Save::~Save()
{
  _close();
}

plugin::Plugin * Save::copy() const
{
  Save * out = new Save;
  out->_option = _option;
  return out;
}

String Save::name() const
{
  return jpeg::name;
}

List<String> Save::extension_list() const
{
  return jpeg::extension_list;
}

//------------------------------------------------------------------------------
// Save::option(), Save::option_list(), Save::option_value(),
// Save::option_default()
//------------------------------------------------------------------------------

bool Save::option(const String & in, String * data)
{
  try
  {
    if (string::compare_no_case(in, option_list()[SAVE_QUALITY]))
      *data >> _option.quality;
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
  
  if (string::compare_no_case(in, option_list()[SAVE_QUALITY]))
    out << _option.quality;
  
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
      case SAVE_QUALITY: out << label_quality; break;
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
      case SAVE_QUALITY: out << _option.quality; break;
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
  
  pixel::FORMAT format = pixel::format(info.pixel);
  switch (format)
  {
    case pixel::LA: format = pixel::L; break;
    case pixel::RGBA: format = pixel::RGB; break;
    default: break;
  }
  _info.pixel = pixel::pixel(format, pixel::U8);
  
  DJV_DEBUG_PRINT("info = " << _info);
  
  _image.set(_info);
}

namespace {

bool init(jpeg_compress_struct * jpeg, Jpeg_Error * error)
{
  if (setjmp(error->jump))
    return false;
  
  libjpeg::jpeg_create_compress(jpeg);
  
  return true;
}

bool open(
  FILE * f,
  jpeg_compress_struct * jpeg,
  const io::Info & info,
  int quality,
  Jpeg_Error * error
) {
  if (setjmp(error->jump))
    return false;

  libjpeg::jpeg_stdio_dest(jpeg, f);
  
  jpeg->image_width = info.size.x;
  jpeg->image_height = info.size.y;
  
  if (pixel::L_U8 == info.pixel)
  {
    jpeg->input_components = 1;
    jpeg->in_color_space = libjpeg::JCS_GRAYSCALE;
  }
  else if (pixel::RGB_U8 == info.pixel)
  {
    jpeg->input_components = 3;
    jpeg->in_color_space = libjpeg::JCS_RGB;
  }
  
  libjpeg::jpeg_set_defaults(jpeg);
  libjpeg::jpeg_set_quality(jpeg, quality, TRUE);
  libjpeg::jpeg_start_compress(jpeg, TRUE);
  
  String _tag = info.tag[tag::label_tag()[tag::DESCRIPTION]];
  if (_tag.size())
    libjpeg::jpeg_write_marker(
      jpeg,
      JPEG_COM,
      (libjpeg::JOCTET *)_tag.c_str(),
      static_cast<uint>(_tag.size())
    );
  
  return true;
}

}

void Save::_open(const String & in, const io::Info & info) throw (Error)
{
  DJV_DEBUG("Save::_open");
  DJV_DEBUG_PRINT("in = " << in);
  
  _close();
  
  // Initialize libjpeg.
  
  _jpeg.err = libjpeg::jpeg_std_error(&_jpeg_error.pub);
  _jpeg_error.pub.error_exit = djv_jpeg_error;
  _jpeg_error.pub.emit_message = djv_jpeg_warning;
  _jpeg_error.msg[0] = 0;
  
  if (! jpeg::init(&_jpeg, &_jpeg_error))
    throw Error(name(), _jpeg_error.msg);
  
  _jpeg_init = true;
  
  // Open.

#if defined(DJV_WINDOWS)
  ::fopen_s(&_f, in.c_str(), "wb");
#else
  _f = ::fopen(in.c_str(), "wb");
#endif
  if (! _f)
    djv_image::io::throw_error_open(name(), in);
  
  if (! jpeg::open(_f, &_jpeg, info, _option.quality, &_jpeg_error))
    throw Error(name(), _jpeg_error.msg);
}

void Save::_close()
{
  if (_jpeg_init)
  {
    libjpeg::jpeg_destroy_compress(&_jpeg);
    _jpeg_init = false;
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

bool scanline(
  libjpeg::jpeg_compress_struct * jpeg,
  const uint8_t * in,
  Jpeg_Error * error
) {
  if (::setjmp(error->jump))
    return false;
  
  libjpeg::JSAMPROW p [] = { (libjpeg::JSAMPLE *)(in) };
  
  if (! libjpeg::jpeg_write_scanlines(jpeg, p, 1))
    return false;

  return true;
}

bool end(libjpeg::jpeg_compress_struct * jpeg, Jpeg_Error * error)
{
  if (::setjmp(error->jump))
    return false;

  libjpeg::jpeg_finish_compress(jpeg);

  return true;
}

}

void Save::set(const Image & in, const io::Frame_Info & frame) throw (Error)
{
  DJV_DEBUG("Save::set");
  DJV_DEBUG_PRINT("in = " << in);
  
  // Open.
  
  const String file_name = _file.get(frame.frame);
  
  DJV_DEBUG_PRINT("file name = " << file_name);

  io::Info info(_info);
  info.tag = in.tag;
  
  _open(file_name, info);
  
  // Write.
  
  const Data * p = &in;

  if (in.info() != _info)
  {
    DJV_DEBUG_PRINT("convert = " << _image);

    _image.zero();

    gl_data::copy(in, &_image);

    p = &_image;
  }
  
  const int h = p->h();
  for (int y = 0; y < h; ++y)
    if (! scanline(&_jpeg, p->data(0, h - 1 - y), &_jpeg_error))
      throw Error(name(), _jpeg_error.msg);
  
  if (! end(&_jpeg, &_jpeg_error))
    throw Error(name(), _jpeg_error.msg);

  _close();
}

//------------------------------------------------------------------------------

}
}

