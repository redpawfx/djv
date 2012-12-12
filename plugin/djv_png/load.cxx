/*------------------------------------------------------------------------------
 plugin/djv_png/load.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "load.h"

namespace djv_plugin {
namespace png {

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
  _png(0),
  _png_info(0),
  _png_info_end(0)
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
  return png::name;
}

List<String> Load::extension_list() const
{
  return png::extension_list;
}

//------------------------------------------------------------------------------
// Load::open(), Load::_open(), Load::_close()
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

bool open(
  FILE * f,
  png_structp png,
  png_infop * png_info,
  png_infop * png_info_end
) {
  if (setjmp(png_jmpbuf(png)))
    return false;
  
  *png_info = png_create_info_struct(png);
  if (! *png_info)
    return false;

  *png_info_end = png_create_info_struct(png);
  if (! *png_info_end)
    return false;
  
  uint8_t tmp [8];
  if (::fread(tmp, 8, 1, f) != 1)
    return false;
  
  if (png_sig_cmp(tmp, 0, 8))
    return false;
  
  png_init_io(png, f);
  png_set_sig_bytes(png, 8);
  png_read_info(png, *png_info);

  if (png_get_interlace_type(png, *png_info) != PNG_INTERLACE_NONE)
    return false;

  png_set_expand(png);
  png_set_gray_1_2_4_to_8(png);
  png_set_palette_to_rgb(png);
  png_set_tRNS_to_alpha(png);
    
  return true;
}

}

void Load::_open(const String & in, io::Info & info) throw (Error)
{
  DJV_DEBUG("Load::_open");
  DJV_DEBUG_PRINT("in = " << in);
  
  _close();

  // Initialize libpng.
  
  _png = png_create_read_struct(
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
  ::fopen_s(&_f, in.c_str(), "rb");
#else
  _f = ::fopen(in.c_str(), "rb");
#endif
  if (! _f)
    djv_image::io::throw_error_open(name(), in);

  if (! png::open(_f, _png, &_png_info, &_png_info_end))
    throw Error(name(), _png_error.msg);
    
  // Information.
  
  info.file_name = in;
  
  info.size = V2i(
    png_get_image_width(_png, _png_info),
    png_get_image_height(_png, _png_info)
  );
  
  int channels = png_get_channels(_png, _png_info);
  if (png_get_color_type(_png, _png_info) == PNG_COLOR_TYPE_PALETTE)
    channels = 3;
  if (png_get_valid(_png, _png_info, PNG_INFO_tRNS))
    ++channels;
  
  DJV_DEBUG_PRINT("channels = " << channels);
  
  int bit_depth = png_get_bit_depth(_png, _png_info);
  if (bit_depth < 8)
    bit_depth = 8;
  
  DJV_DEBUG_PRINT("bit depth = " << bit_depth);
  
  if (bit_depth >= 16 && memory::LSB == memory::endian())
    png_set_swap(_png);
  
  if (! pixel::pixel_(channels, bit_depth, false, &info.pixel))
    djv_image::io::throw_error_unsupported(name(), in);
}

void Load::_close()
{
  if (_png || _png_info || _png_info_end)
  {
    png_destroy_read_struct(
      _png ? &_png : 0,
      _png_info ? &_png_info : 0,
      _png_info_end ? &_png_info_end : 0
    );
    _png = 0;
    _png_info = 0;
    _png_info_end = 0;
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

bool scanline(png_structp png, uint8_t * out)
{
  if (setjmp(png_jmpbuf(png)))
    return false;
  
  png_read_row(png, out, 0);
  
  return true;
}

bool end(png_structp png, png_infop png_info)
{
  if (setjmp(png_jmpbuf(png)))
    return false;
  
  png_read_end(png, png_info);
  
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
  
  // Read.
  
  Data * data = frame.proxy ? &_tmp : &_image;
  data->set(info);
  
  for (int y = 0; y < info.size.y; ++y)
    if (! scanline(_png, data->data(0,  data->h() - 1 - y)))
      throw Error(name(), _png_error.msg);

  end(_png, _png_info_end);
  
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

