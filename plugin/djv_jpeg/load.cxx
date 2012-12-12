/*------------------------------------------------------------------------------
 plugin/djv_jpeg/load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "load.h"

namespace djv_plugin {
namespace jpeg {
using libjpeg::jpeg_decompress_struct;

//------------------------------------------------------------------------------
// djv_image_load()
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT plugin::Plugin * djv_image_load()
{
  return new Load;
}

}

//------------------------------------------------------------------------------
// Load
//------------------------------------------------------------------------------

Load::Load() :
  _f(0),
  _jpeg_init(false)
{}

Load::~Load()
{
  _close();
}

plugin::Plugin * Load::copy() const
{
  return new Load;
}

String Load::name() const
{
  return jpeg::name;
}

List<String> Load::extension_list() const
{
  return jpeg::extension_list;
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

namespace {

bool init(libjpeg::jpeg_decompress_struct * jpeg, Jpeg_Error * error)
{
  if (::setjmp(error->jump))
    return false;
  
  libjpeg::jpeg_create_decompress(jpeg);
  
  return true;
}

bool open(FILE * f, libjpeg::jpeg_decompress_struct * jpeg, Jpeg_Error * error)
{
  if (::setjmp(error->jump))
    return false;
  
  libjpeg::jpeg_stdio_src(jpeg, f);
  
  libjpeg::jpeg_save_markers(jpeg, JPEG_COM, 0xFFFF);
  
  if (! libjpeg::jpeg_read_header(jpeg, TRUE))
    return false;
  
  if (! libjpeg::jpeg_start_decompress(jpeg))
    return false;
  
  return true;
}

}

void Load::_open(const String & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::_open");
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
  ::fopen_s(&_f, in.c_str(), "rb");
#else
  _f = ::fopen(in.c_str(), "rb");
#endif
  if (! _f)
    djv_image::io::throw_error_open(name(), in);

  if (! jpeg::open(_f, &_jpeg, &_jpeg_error))
    throw Error(name(), _jpeg_error.msg);
  
  // Information.
  
  info.file_name = in;
  
  info.size = V2i(_jpeg.output_width, _jpeg.output_height);
  
  if (! pixel::pixel_(_jpeg.out_color_components, 8, false, &info.pixel))
    djv_image::io::throw_error_unsupported(name(), in);
  
  // Image tags.
  
  const libjpeg::jpeg_saved_marker_ptr marker = _jpeg.marker_list;
  
  if (marker)
    info.tag[tag::label_tag()[tag::DESCRIPTION]] =
      String((const char *)marker->data, marker->data_length);

  DJV_DEBUG_PRINT("info = " << info);
}

void Load::_close()
{
  if (_jpeg_init)
  {
    libjpeg::jpeg_destroy_decompress(&_jpeg);
    _jpeg_init = false;
  }
  
  if (_f)
  {
    ::fclose(_f);
    _f = 0;
  }
}

//------------------------------------------------------------------------------
// Load::get()
//------------------------------------------------------------------------------

namespace {

bool scanline(
  libjpeg::jpeg_decompress_struct * jpeg,
  uint8_t * out,
  Jpeg_Error * error
) {
  if (::setjmp(error->jump))
    return false;
  
  libjpeg::JSAMPROW p [] = { (libjpeg::JSAMPLE *)(out) };
  
  if (! libjpeg::jpeg_read_scanlines(jpeg, p, 1))
    return false;
  
  return true;
}

bool end(
  libjpeg::jpeg_decompress_struct * jpeg,
  Jpeg_Error * error
) {
  if (::setjmp(error->jump))
    return false;
  
  libjpeg::jpeg_finish_decompress(jpeg);
  
  return true;
}

}

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
  
  // Read.
  
  Data * data = frame.proxy ? &_tmp : &_image;
  data->set(info);
  
  for (int y = 0; y < info.size.y; ++y)
    if (! scanline(&_jpeg, data->data(0, data->h() - 1 - y), &_jpeg_error))
      throw Error(name(), _jpeg_error.msg);
  
  if (! end(&_jpeg, &_jpeg_error))
    throw Error(name(), _jpeg_error.msg);
  
  if (frame.proxy)
  {
    info.size = data::proxy_scale(info.size, frame.proxy);
    info.proxy = frame.proxy;
    _image.set(info);
    
    data::proxy_scale(_tmp, &_image, frame.proxy);
  }

  DJV_DEBUG_PRINT("image = " << _image);
  
  return &_image;
}

//------------------------------------------------------------------------------

}
}

